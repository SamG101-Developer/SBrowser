#include "form_internals.hpp"

#include <tuple>

#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>

#include <html/elements/html_button_element.hpp>
#include <html/elements/html_form_element.hpp>

#include <html/helpers/html_element_internals.hpp>
#include <html/mixins/form_associated.hpp>


auto html::helpers::form_internals::supported_property_names(
        elements::html_form_element* form_element)
        -> ext::string_vector
{
    ext::vector<std::tuple<ext::string, dom::nodes::element*, ext::string, int>> sources_names;
    // TODO
}


auto html::helpers::form_internals::pick_encoding_for_form(
        elements::html_form_element* form_element)
        -> ext::string
{
    ext::string encoding = form_element->owner_document->character_set;
    if (ext::string input = form_element->accept_charset)
    {
        auto candidate_encoding_labels = input.split(' ', input.count(' '));
        auto candidate_encodings = candidate_encoding_labels
                .transform<>([](const ext::string& encoding_label) {return encoding::helpers::names_labels::get_encoding(encoding_label);})
                .remove("");

        return candidate_encodings.empty() ? "UTF-8" : candidate_encodings.front();
    }

    return encoding::helpers::names_labels::get_output_encoding(encoding);
}


template <typename T>
auto html::helpers::form_internals::submit(
        elements::html_form_element* form_element,
        mixins::form_associated<T>* submitter,
        bool submitted_from_submit_flag)
        -> void
{
    // TODO : cannot navigate?

    if (form_element->m_constructing_entry_list)
        return;

    if (form_element->owner_document->m_sandboxing_flag & internal::sandboxing_flags::SANDBOXED_FORMS)
        return;

    auto* form_browsing_context = form_element->owner_document->m_browsing_context;
    if (not submitted_from_submit_flag)
    {
        if (form_element->m_firing_submission_events)
            return;
        form_element->m_firing_submission_events = true;

        if (not static_cast<T*>(submitter)->no_validate)
        {
            auto validation_result = interactively_validate_constraints(form_element);
            if (validation_result < 0)
            {
                form_element->m_firing_submission_events = false;
                return;
            }
        }

//        auto* submitter_button
    }
}


template<typename T>
auto html::helpers::form_internals::reset_form_owner(
        mixins::form_associated<T>* form_associated_element)
        -> void
{
    // unset the parser flag for the form associated element, and return if there is no form associated
    form_associated_element->m_parser_inserted_flag = false;
    if (not form_associated_element->form
            and /* TODO */ true
            and /* TODO */ true)
        return;

    // if the element is listed and is shadow-connected
    if (helpers::html_element_internals::is_listed_element(form_associated_element)
            and dom::helpers::shadows::is_connected(form_associated_element))
    {
        // the new form element is the first form in the tree to have a matching id to
        auto* form_element = dom::helpers::trees::descendants(dom::helpers::trees::root(form_associated_element))
                .template cast_all<dom::nodes::element*>()
                .template first_match([form_associated_element](dom::nodes::element* element) {return element->id == form_associated_element->form->id;})
                .template cast_all<elements::html_form_element*>()
                .front();

        // if the form element exists, set it, otherwise set it to the default nullptr
        form_associated_element->form = form_element;
    }

    else
    {
        // the ew form element is the first form ancestor (TODO : reverse list?)
        auto* form_ancestor = dom::helpers::trees::ancestors(form_associated_element)
                .template call_all<elements::html_form_element*>()
                .front();

        // if the form element exists, set it, otherwise set it to the default nullptr
        form_associated_element->form = form_ancestor;
    }
}


template <typename T>
auto html::helpers::form_internals::statically_validate_constraints(
        mixins::form_associated<T>* form_associated_element)
        -> short
{
    auto controls = dom::helpers::trees::descendants(dom::helpers::trees::root(form_associated_element))
            .filter(&helpers::html_element_internals::is_submittable_element)
            .filter([form_associated_element](mixins::form_associated<T>* form_associated_sub_element) {return form_associated_sub_element->form == form_associated_element->form;});

    ext::vector<html_element_internals*> invalid_controls;

    // TODO
}


auto html::helpers::form_internals::no_validate_state(
        elements::html_element* element)
        -> bool
{
    auto* button_element = dynamic_cast<elements::html_button_element*>(element);
    auto* form_associated_element = dynamic_cast<mixins::form_associated<T>>(element);
}
