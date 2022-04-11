#include "script_internals.hpp"

#include <ext/string_switch.hpp>
#include <javascript/environment/realms.hpp>

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/shadows.hpp>

#include <dom/nodes/document.hpp>

#include <html/elements/html_script_element.hpp>
#include <html/helpers/document_internals.hpp>


auto html::helpers::script_internals::prepare_script(
        elements::html_script_element* script_element)
        -> void
{
    // return if the script element has already started
    if (script_element->m_already_started_flag)
        return;

    // get the parser document from the script and set the script's parser document to nullptr
    dom::nodes::document* parser_document = nullptr;
    std::swap(parser_document, script_element->m_parser_document);

    // if there is a parser document and the script isn't async, then set the non-blocking flag to true
    if (parser_document and not script_element->async)
        script_element->m_non_blocking_flag = true;

    // get the source text of the string, and return if there is no source or source text
    ext::string source_text = script_element->text;
    if (not script_element->src and not source_text)
        return;

    // return if the script element isn't shadow-connected TODO : why?
    if (not dom::helpers::shadows::is_connected(script_element))
        return;

    // decide on the type of script block based on the type and lang attributes of the script_element
    ext::string script_type;
    if (not script_element->type and not script_element->lang)
        // no type or language means a default of 'text/javascript' is used as the script type
        script_type = "text/javascript";
    else if (script_element->type)
        // only a type means that the type attribute can be used
        script_type = script_element->type;
    else if (script_element->lang)
        // only a lang means that the type if 'text/' with the language appended
        script_type = "text/" + script_element->lang;


    // decide on the type attribute of the script element based on the script_type
    if (infra::helpers::strings:strip_leading_trailing_whitespace(script_element->type).endswith("/javascript"))
        // if the script type ends with '/javascript', then set the type attribute to 'classic'
        script_element->type = "classic";
    else if (script_type.new_lowercase() == "module")
        // if the script type is 'module', then set the type attribute to 'module'
        script_element->type = "module";
    else
        // return if the 1 of the other 2 conditions aren't met, as other types aren't supported
        return;

    // set the documents and flags to the script element
    script_element->m_parser_document = parser_document;
    script_element->m_non_blocking_flag = false;
    script_element->m_already_started_flag = true;
    script_element->m_preparation_time_document = script_element->owner_document;

    // return if the parser document isn't the same as the preparation time document
    if (parser_document and parser_document != script_element->m_preparation_time_document)
        return;

    // return if scripting is disabled for this element (manually turned off by user etc)
    if (is_scripting_disabled(script_element))
        return;

    // return if the element has 'no_module' set and the script isn't a module ie is 'classic'
    if (script_element->no_module and script_element->type == "classic")
        return;

    // return if there is no source attribute and the content security policy algorithm blocks it
    if (not script_element->src and csp::helpers::integration_internals::should_elements_inline_type_behaviour_be_blocked(script_element, "script", source_text) == csp::helpers::integration_internals::BLOCKED)
        return;

    // create the script fetch options from information about the script element
    script_fetch_options options
            {
        .cryptographic_nonce = script_element->s_cryptographic_nonce,
        .integrity_metadata = script_element->integrity,
        .parser_metadata = is_parser_inserted(script_element) ? "parser-inserted" : "not-parser-inserted",
        .credentials_mode = script_element->cross_origin, // TODO : map value
        .referrer_policy = script_element->referrer_policy
            };

    // get the cors settings and settings object for the script element
    ext::string classic_script_cors_setting = script_element->cross_origin;
    auto settings_object = javascript::realms::relevant_realm(script_element->owner_document).settings_object;

    // handle if the element has a source attribute
    if (script_element->src)
    {
        // handle async changes to the source
        if (not script_element->src)
        {
            // return (and fire an error event) if the source is now empty
            dom::helpers::mutation_observers::queue_element_task(script_element, [&script_element] {dom::helpers::event_dispatching::fire_event<>("error", script_element);});
            return;
        }

        // set the external file flag, and get the url (and return code) from the parsing algorithm
        script_element->m_from_external_file_flag = true;
        ext::string result; // TODO : from parsing
        ext::string url; // TODO : from parsing

        // queue an element task to fire an error event at the script element if the parsing failed
        if (result == "failure")
            dom::helpers::mutation_observers::queue_element_task(script_element, [&script_element] {dom::helpers::event_dispatching::fire_event<>("error", script_element);});

        // block the rendering of the script element if the script element is render blocking
        if (document_internals::is_render_blocking(script_element))
            document_internals::block_rendering(script_element->owner_document, script_element); // TODO : check document argument

        // fetch a classic script if the script type is 'classic'
        if (script_element->type == "classic")
            fetch_classic_script(url, settings_object, options, classic_script_cors_setting);

        // fetch an external module script if the script type is 'module'
        else if (script_element->type == "module")
            fetch_external_module_script_graph(url, settings_object, options);
    }

    else
    {
        // get the url from the owner document
        ext::string url = script_element->owner_document->m_document_base_url;

        // create a classic script if the script type is 'classic'
        if (script_element->type == "classic")
            script_element->m_script = create_classic_script(source_text, settings_object, url, options);

        // fetch an inline module script if the script type is 'module'
        else if (script_element->type == "module")
            script_element->m_script = fetch_inline_module_script_graph(source_text, url, settings_object, options);
    }

    // TODO : comment

    if (script_element->type == "classic" and script_element->src and script_element->defer and not script_element->async and is_parser_inserted(script_element))
        script_element->owner_document->m_list_of_scripts_that_will_execute_post_parsing.append(script_element); // TODO : correct document?

    else if (script_element->type == "module" and not is_parser_inserted(script_element) and is_parser_inserted(script_element))
        script_element->owner_document->m_list_of_scripts_that_will_execute_post_parsing.append(script_element); // TODO : correct document?

    else if (script_element->type == "classic" and script_element->src and not script_element->async and is_parser_inserted(script_element))
        document_internals::block_rendering(script_element->owner_document, script_element); // TODO : correct document?

    else if (script_element->type == "classic" and script_element->src and not script_element->async and not script_element->m_non_blocking_flag)
        script_element->m_preparation_time_document->m_list_of_scripts_that_will_execute_post_parsing.append(script_element);

    else if (script_element->type == "module" and not script_element->async and not script_element->m_non_blocking_flag)
        script_element->m_preparation_time_document->m_list_of_scripts_that_will_execute_post_parsing.append(script_element);

    else if (script_element->type == "classic" and script_element->src)
        script_element->m_preparation_time_document->m_list_of_scripts_that_will_execute_soon_as_possible.append(script_element);

    else if (script_element->type == "module")
        script_element->m_preparation_time_document->m_list_of_scripts_that_will_execute_soon_as_possible.append(script_element);

    else if (script_element->src and is_parser_inserted(script_element) /* TODO : some more conditions */)
        script_element->m_ready_to_be_parser_executed_flag = true;

    else
        execute_script_block(script_element); // TODO : immediately, even if other scripts are already executing (in parallel?)
}


auto html::helpers::script_internals::execute_script_block(
        elements::html_script_element* script_element)
        -> void
{
    // get the owner document of the script
    dom::nodes::document* document = script_element->owner_document;

    // return if the preparation time document isn't the same as the owner document
    if (script_element->m_preparation_time_document != script_element->owner_document)
        return;

    // unblock the rendering of the script element
    document_internals::unblock_rendering(script_element->owner_document, script_element);

    // handle not having an m_script attribute set on the script element
    if (not script_element->m_script)
    {
        // return (and fire an error event at the script element)
        dom::helpers::event_dispatching::fire_event<>("error", script_element);
        return;
    }

    // set a boolean for recording whether the m_ignore_destructive_writes_counter has incremented on the document
    bool has_incremented = false;

    // handle a script that is from an external file or a module
    if (script_element->m_from_external_file_flag or script_element->type == "module")
    {
        // set the incremented variable to true, and increment the document's m_ignore_destructive_writes_counter
        has_incremented = true;
        document->m_ignore_destructive_writes_counter += 1;
    }

    // handle classic script elements
    if (script_element->type == "classic")
    {
        // save the current script of the document
        elements::html_script_element* old_current_script = document->current_script;

        // if the root of the script element is a shadow root, set the document's current_script to the script_element,
        // otherwise set it to nullptr
        document->current_script = dom::helpers::shadows::is_root_shadow_root(script_element)
                ? script_element
                : nullptr;

        // run the classic script
        run_classic_script(script_element->m_script);

        // restore the saved current script of the document
        document->current_script = old_current_script;
    }

    // handle module script elements
    else if (script_element->type == "module")
    {
        // assert that there is no current script on the document
        assert(not document->current_script);

        // run the module script
        run_module_script(script_element->m_script);
    }

    // if the counter was incremented, then decrement it
    if (has_incremented)
        document->m_ignore_destructive_writes_counter -= 1;

    // if the script is from an external file, fire the 'load' event at the script element
    if (script_element->m_from_external_file_flag)
        dom::helpers::event_dispatching::fire_event<>("load", script_element);
}
