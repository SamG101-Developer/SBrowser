#ifndef SBROWSER_EXPOSE_CPP_TO_JS_HPP
#define SBROWSER_EXPOSE_CPP_TO_JS_HPP

#include <javascript/environment/modules.hpp>
#include <javascript/interop/javascript_namespace.hpp>

#include <dom/aborting/abort_controller.hpp>
#include <dom/aborting/abort_signal.hpp>

#include <dom/events/event.hpp>
#include <dom/events/custom_event.hpp>

#include <dom/iterators/abstract_iterator.hpp>
#include <dom/iterators/node_filter.hpp>
#include <dom/iterators/node_iterator.hpp>
#include <dom/iterators/tree_walker.hpp>

#include <dom/mixins/child_node.hpp>
#include <dom/mixins/document_or_element_node.hpp>
#include <dom/mixins/document_or_shadow_root.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>
#include <dom/mixins/non_element_parent_node.hpp>
#include <dom/mixins/parent_node.hpp>
#include <dom/mixins/slottable.hpp>

#include <dom/mutations/mutation_observer.hpp>
#include <dom/mutations/mutation_record.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/cdata_section.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/comment.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/event_target.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/processing_instruction.hpp>
#include <dom/nodes/shadow_root.hpp>
#include <dom/nodes/text.hpp>
#include <dom/nodes/window.hpp>
#include <dom/nodes/window_proxy.hpp>
#include <dom/nodes/xml_document.hpp>

#include <dom/other/dom_exception.hpp>
#include <dom/other/dom_implementation.hpp>
#include <dom/other/xslt_processor.hpp>
#include <dom/other/xslt_processor.hpp>

#include <dom/ranges/abstract_range.hpp>
#include <dom/ranges/range.hpp>
#include <dom/ranges/static_range.hpp>

#include <dom/xpath/xpath_evaluator.hpp>
#include <dom/xpath/xpath_expression.hpp>
#include <dom/xpath/xpath_result.hpp>

#include <geometry/shapes/dom_rect.hpp>

#include <html/elements/html_element.hpp>

#include <v8.h>
#include <v8pp/class.hpp>
#include <v8pp/module.hpp>


namespace javascript::interop::expose_cpp_to_js {
    template <typename T> auto object_to_v8(v8::Isolate* isolate)  requires std::is_base_of_v<dom_object, T>;
    void expose(v8::Isolate* isolate, v8::Persistent<v8::Context>& persistent_context, javascript::environment::modules::module_type module_type);
}


template <typename T>
auto javascript::interop::expose_cpp_to_js::object_to_v8(
        v8::Isolate* isolate) requires std::is_base_of_v<dom_object, T> {

    return T{}.v8(isolate).template to<v8pp::class_<T>>();
}


void
javascript::interop::expose_cpp_to_js::expose(
        v8::Isolate* isolate,
        v8::Persistent<v8::Context>& persistent_context,
        javascript::environment::modules::module_type module_type) {

    v8::Local<v8::Context> local_context = v8::Local<v8::Context>::New(isolate, persistent_context);
    local_context->Enter();

    v8pp::class_<dom::mixins::child_node<dom::nodes::node>> v8_child_node{isolate};
    v8_child_node
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::document_or_element_node<dom::nodes::node>> v8_document_or_element_node{isolate};
    v8_document_or_element_node
            .function("getElementsByTagName", &dom::mixins::document_or_element_node<dom::nodes::node>::get_elements_by_tag_name)
            .function("getElementsByTagNameNS", &dom::mixins::document_or_element_node<dom::nodes::node>::get_elements_by_tag_name_ns)
            .function("getElementsByClassName", &dom::mixins::document_or_element_node<dom::nodes::node>::get_elements_by_class_name)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::document_or_shadow_root<dom::nodes::node>> v8_document_or_shadow_root{isolate};
    v8_document_or_shadow_root
            .var("activeElement", &dom::mixins::document_or_shadow_root<dom::nodes::node>::active_element)
            .var("styleSheets", &dom::mixins::document_or_shadow_root<dom::nodes::node>::style_sheets)
            .var("adoptedStyleSheets", &dom::mixins::document_or_shadow_root<dom::nodes::node>::adopted_style_sheets)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::non_document_type_child_node<dom::nodes::node>> v8_non_document_type_child_node{isolate};
    v8_non_document_type_child_node
            .var("previousElementSibling", &dom::mixins::non_document_type_child_node<dom::nodes::node>::previous_element_sibling)
            .var("nextElementSibling", &dom::mixins::non_document_type_child_node<dom::nodes::node>::next_element_sibling)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::non_element_parent_node<dom::nodes::node>> v8_non_element_parent_node{isolate};
    v8_non_element_parent_node
            .function("getElementById", &dom::mixins::non_element_parent_node<dom::nodes::node>::get_element_by_id)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::parent_node<dom::nodes::node>> v8_parent_node{isolate};
    v8_parent_node
//            .function("prepend", &dom::mixins::parent_node<dom::nodes::node>::prepend)
//            .function("append", &dom::mixins::parent_node<dom::nodes::node>::append)
//            .function("replaceChildren", &dom::mixins::parent_node<dom::nodes::node>::replace_children)
            .function("querySelector", &dom::mixins::parent_node<dom::nodes::node>::query_selector)
            .function("querySelectorAll", &dom::mixins::parent_node<dom::nodes::node>::query_selector_all)

            .var("children", &dom::mixins::parent_node<dom::nodes::node>::children)
            .var("firstElementChild", &dom::mixins::parent_node<dom::nodes::node>::first_element_child)
            .var("lastElementChild", &dom::mixins::parent_node<dom::nodes::node>::last_element_child)
            .var("childElementCount", &dom::mixins::parent_node<dom::nodes::node>::child_element_count)
            .auto_wrap_objects();

    v8pp::class_<dom::mixins::slottable<dom::nodes::node>> v8_slottable{isolate};
    v8_slottable
            .var("assignedSlot", &dom::mixins::slottable<dom::nodes::node>::assigned_slot)
            .auto_wrap_objects();

    v8pp::class_<dom::mutations::mutation_record> v8_mutation_record{isolate};
    v8_mutation_record
            .var("type", &dom::mutations::mutation_record::type)
            .var("target", &dom::mutations::mutation_record::target)
            .var("addedNodes", &dom::mutations::mutation_record::added_nodes)
            .var("removedNodes", &dom::mutations::mutation_record::removed_nodes)
            .var("previousSibling", &dom::mutations::mutation_record::previous_sibling)
            .var("nextSibling", &dom::mutations::mutation_record::next_sibling)
            .var("attributeName", &dom::mutations::mutation_record::attribute_name)
            .var("attributeNamespace", &dom::mutations::mutation_record::attribute_namespace)
            .var("oldValue", &dom::mutations::mutation_record::old_value)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::attr> v8_attr{isolate};
    v8_attr
            .inherit<dom::nodes::node>()
            .var("namespaceURI", &dom::nodes::attr::namespace_uri, true)
            .var("prefix", &dom::nodes::attr::prefix, true)
            .var("localName", &dom::nodes::attr::local_name, true)
            .var("name", &dom::nodes::attr::name, true)
            .var("value", &dom::nodes::attr::value)
            .var("ownerElement", &dom::nodes::attr::owner_element, true);

    v8pp::class_<dom::nodes::cdata_section> v8_cdata_section{isolate};
    v8_cdata_section
            .inherit<dom::nodes::text>()
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::character_data> v8_character_data{isolate};
    v8_character_data
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::child_node<dom::nodes::character_data>>()
            .inherit<dom::mixins::non_document_type_child_node<dom::nodes::character_data>>()

            .function("substringData", &dom::nodes::character_data::substring_data)
            .function("appendData", &dom::nodes::character_data::append_data)
            .function("insertData", &dom::nodes::character_data::insert_data)
            .function("replaceData", &dom::nodes::character_data::replace_data)
            .function("deleteData", &dom::nodes::character_data::delete_data)
            .var("data", &dom::nodes::character_data::data)
            .var("length", &dom::nodes::character_data::length, true)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::comment> v8_comment{isolate};
    v8_comment
            .ctor<>()
            .ctor<ext::cstring&>()
            .inherit<dom::nodes::character_data>()
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::document> v8_document{isolate};
    v8_document
            .ctor<>()
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::document_or_element_node<dom::nodes::document>>()
            .inherit<dom::mixins::document_or_shadow_root<dom::nodes::document>>()
            .inherit<dom::mixins::parent_node<dom::nodes::document>>()
            .inherit<dom::mixins::non_element_parent_node<dom::nodes::document>>()
            .inherit<dom::xpath::xpath_evaluator>()
            .inherit<ext::listlike<dom::nodes::node*>>()

            .function("createElement", &dom::nodes::document::create_element)
            .function("createElementNS", &dom::nodes::document::create_element_ns)
            .function("createDocumentFragment", &dom::nodes::document::create_document_fragment)
            .function("createTextNode", &dom::nodes::document::create_text_node)
            .function("createCDataSectionNode", &dom::nodes::document::create_cdata_section_node)
            .function("createComment", &dom::nodes::document::create_comment)
            .function("createProcessingInstruction", &dom::nodes::document::create_processing_instruction)
            .function("createAttribute", &dom::nodes::document::create_attribute)
            .function("createAttributeNS", &dom::nodes::document::create_attribute_ns)
            .function("createRange", &dom::nodes::document::create_range)
            .function("createNodeIterator", &dom::nodes::document::create_node_iterator)
            .function("createTreeWalker", &dom::nodes::document::create_tree_walker)
            .function("importNode", &dom::nodes::document::import_node)
            .function("adoptNode", &dom::nodes::document::adopt_node)

            .function("getElementsByName", &dom::nodes::document::get_elements_by_name)
//            .function("open", &dom::nodes::document::open)
            .function("close", &dom::nodes::document::close)
//            .function("write", &dom::nodes::document::write)
//            .function("writeln", &dom::nodes::document::writeln)
            .function("hasFocus", &dom::nodes::document::has_focus)
            .function("execCommand", &dom::nodes::document::exec_command)
            .function("queryCommandEnabled", &dom::nodes::document::query_command_enabled)
            .function("queryCommandIndeterm", &dom::nodes::document::query_command_indeterm)
            .function("queryCommandState", &dom::nodes::document::query_command_state)
            .function("queryCommandSupported", &dom::nodes::document::query_command_supported)
            .function("queryCommandValue", &dom::nodes::document::query_command_value)

            .function("elementFromPoint", &dom::nodes::document::element_from_point)
            .function("elementsFromPoint", &dom::nodes::document::elements_from_point)
            .function("caretPositionFromPoint", &dom::nodes::document::caret_position_from_point)

            .var("URL", &dom::nodes::document::url, true)
            .var("compatMode", &dom::nodes::document::compat_mode, true)
            .var("characterSet", &dom::nodes::document::character_set, true)
            .var("contentType", &dom::nodes::document::content_type, true)
            .var("doctype", &dom::nodes::document::doctype, true)
            .var("documentElement", &dom::nodes::document::document_element, true)
            .var("implementation", &dom::nodes::document::implementation, true)

            .var("domain", &dom::nodes::document::domain, true)
            .var("cookie", &dom::nodes::document::cookie)
            .var("referrer", &dom::nodes::document::referrer, true)
            .var("lastModified", &dom::nodes::document::last_modified, true)
            .var("readyState", &dom::nodes::document::ready_state, true)
            .var("dir", &dom::nodes::document::dir)
            .var("designMode", &dom::nodes::document::design_mode)
            .var("title", &dom::nodes::document::title)
            .var("location", &dom::nodes::document::location)
            .var("body", &dom::nodes::document::body)
            .var("head", &dom::nodes::document::head, true)
            .var("images", &dom::nodes::document::images, true)
            .var("links", &dom::nodes::document::links, true)
            .var("forms", &dom::nodes::document::forms, true)
            .var("scripts", &dom::nodes::document::scripts, true)
            .var("defaultView", &dom::nodes::document::default_view, true)

            .var("scrollingElement", &dom::nodes::document::scrolling_element)

            .var("namedFlows", &dom::nodes::document::named_flows)

            .auto_wrap_objects();

    v8pp::class_<dom::nodes::document_fragment> v8_document_fragment{isolate};
    v8_document_fragment
            .ctor<>()
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::non_element_parent_node<dom::nodes::document_fragment>>()
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::document_type> v8_document_type{isolate};
    v8_document_type
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::child_node<dom::nodes::document_type>>()

            .var("name", &dom::nodes::document_type::name)
            .var("publicId", &dom::nodes::document_type::public_id)
            .var("systemId", &dom::nodes::document_type::system_id)
            .auto_wrap_objects();


    v8pp::class_<dom::nodes::processing_instruction> v8_processing_instruction{isolate};
    v8_processing_instruction
            .inherit<dom::nodes::node>()
            .var("target", &dom::nodes::processing_instruction::target)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::shadow_root> v8_shadow_root{isolate};
    v8_shadow_root
            .inherit<dom::nodes::document_fragment>()
            .inherit<dom::mixins::document_or_shadow_root<dom::nodes::shadow_root>>()

            .var("mode", &dom::nodes::shadow_root::mode, true)
            .var("delegatesFocus", &dom::nodes::shadow_root::delegates_focus, true)
            .var("slotAssignment", &dom::nodes::shadow_root::slot_assignment, true)
            .var("host", &dom::nodes::shadow_root::host, true)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::text> v8_text{isolate};
    v8_text
            .ctor<>()
            .ctor<ext::cstring&>()
            .inherit<dom::nodes::character_data>()
            .inherit<dom::mixins::slottable<dom::nodes::text>>()

            .function("splitText", &dom::nodes::text::split_text)
            .var("wholeText", &dom::nodes::text::whole_text, true)
            .auto_wrap_objects();

    v8pp::class_<dom::nodes::xml_document> v8_xml_document{isolate};
    v8_xml_document
            .inherit<dom::nodes::document>()
            .auto_wrap_objects();


    auto v8_abort_controller = object_to_v8<dom::aborting::abort_controller>(isolate);
    auto v8_abort_signal = object_to_v8<dom::aborting::abort_signal>(isolate);

    auto v8_custom_event = object_to_v8<dom::events::custom_event>(isolate);
    auto v8_event = object_to_v8<dom::events::event>(isolate);

    auto v8_abstract_iterator = object_to_v8<dom::iterators::abstract_iterator>(isolate);
    auto v8_node_filter = object_to_v8<dom::iterators::node_filter>(isolate);
    auto v8_node_iterator = object_to_v8<dom::iterators::node_iterator>(isolate);
    auto v8_tree_walker = object_to_v8<dom::iterators::tree_walker>(isolate);

    auto v8_mutation_observer = object_to_v8<dom::mutations::mutation_observer>(isolate);

    auto v8_element = object_to_v8<dom::nodes::element>(isolate);
    auto v8_event_target = object_to_v8<dom::nodes::event_target>(isolate);
    auto v8_node = object_to_v8<dom::nodes::node>(isolate);
    auto v8_window_proxy = object_to_v8<dom::nodes::window_proxy>(isolate);
    auto v8_window = object_to_v8<dom::nodes::window>(isolate);

    auto v8_dom_exception = object_to_v8<dom::other::dom_exception>(isolate);
    auto v8_dom_implementation = object_to_v8<dom::other::dom_implementation>(isolate);
    auto v8_xslt_processor = object_to_v8<dom::other::xslt_processor>(isolate);

    auto v8_abstract_range = object_to_v8<dom::ranges::abstract_range>(isolate);
    auto v8_range = object_to_v8<dom::ranges::range>(isolate);
    auto v8_static_range = object_to_v8<dom::ranges::static_range>(isolate);

    auto v8_xpath_evaluator = object_to_v8<dom::xpath::xpath_evaluator>(isolate);
    auto v8_xpath_expression = object_to_v8<dom::xpath::xpath_expression>(isolate);
    auto v8_xpath_result = object_to_v8<dom::xpath::xpath_result>(isolate);

    v8pp::module v8_module{isolate};
    v8::Local<v8::String> module_name;

    switch(module_type) {
        case javascript::environment::modules::module_type::window: {
            v8_module
                    .class_("AbortController", v8_abort_controller)
                    .class_("AbortSignal", v8_abort_signal)

                    .class_("CustomEvent", v8_custom_event)
                    .class_("Event", v8_event)

                    .class_("NodeFilter", v8_node_filter)
                    .class_("NodeIterator", v8_node_iterator)
                    .class_("TreeWalker", v8_tree_walker)

                    .class_("MutationObserver", v8_mutation_observer)
                    .class_("MutationRecord", v8_mutation_record)

                    .class_("Attr", v8_attr)
                    .class_("CDataSection", v8_cdata_section)
                    .class_("CharacterData", v8_character_data)
                    .class_("Comment", v8_comment)
                    .class_("Document", v8_document)
                    .class_("DocumentFragment", v8_document_fragment)
                    .class_("DocumentType", v8_document_type)
                    .class_("Element", v8_element)
                    .class_("EventTarget", v8_event_target)
                    .class_("Node", v8_node)
                    .class_("ProcessingInstruction", v8_processing_instruction)
                    .class_("ShadowRoot", v8_shadow_root)
                    .class_("Text", v8_text)
                    .class_("Window", v8_window)
                    .class_("WindowProxy", v8_window_proxy)
                    .class_("XmlDocument", v8_xml_document)

                    .class_("DomException", v8_dom_exception)
                    .class_("DomImplementation", v8_dom_implementation)
                    .class_("XSLTProcessor", v8_xslt_processor)

                    .class_("AbstractRange", v8_abstract_range)
                    .class_("Range", v8_range)
                    .class_("StaticRange", v8_static_range)

                    .class_("XPathEvaluator", v8_xpath_evaluator)
                    .class_("XPathExpression", v8_xpath_expression)
                    .class_("XPathResult", v8_xpath_result);

            module_name = v8::String::NewFromUtf8(isolate, "Window").ToLocalChecked();
        }

        case javascript::environment::modules::module_type::worker: {
            v8_module
                    .class_("CustomEvent", v8_custom_event)
                    .class_("Event", v8_event)
                    .class_("EventTarget", v8_event_target)
                    .class_("DomException", v8_dom_exception);

            module_name = v8::String::NewFromUtf8(isolate, "Worker").ToLocalChecked();
        }

        case javascript::environment::modules::module_type::audio_worklet: {
            v8_module
                    .class_("Event", v8_event)
                    .class_("EventTarget", v8_event_target);

            module_name = v8::String::NewFromUtf8(isolate, "AudioWorklet").ToLocalChecked();
        }

        default:
            module_name = v8::String::NewFromUtf8(isolate, "Unknown").ToLocalChecked();
    }

    local_context->Global()->Set(local_context, module_name, v8_module.new_instance());
}


#endif //SBROWSER_EXPOSE_CPP_TO_JS_HPP
