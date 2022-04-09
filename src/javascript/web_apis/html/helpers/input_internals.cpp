#include "input_internals.hpp"

#include <dom/helpers/event_dispatching.hpp>

#include <html/elements/html_opt_group_element.hpp>
#include <html/elements/html_option_element.hpp>
#include <html/elements/html_select_element.hpp>


auto html::helpers::input_internals::display_size(
        elements::html_select_element* select_element)
        -> int
{
    // return the size attribute if it exists, otherwise 4 for multiple and 1 for non-multiple
    return select_element->size ? select_element-> size : select_element->multiple ? 4 : 1;
}


auto html::helpers::input_internals::list_of_options(
        elements::html_select_element* select_element)
        -> ext::vector<elements::html_option_element*>
{
    // get the HTMLOptionElement children and HTMLOptionElement grandchildren (HTMLOptGroupElement children)
    auto children = select_element->children->cast_all<elements::html_option_element*>();
    auto grandchildren = select_element->children->cast_all<elements::html_opt_group_element*>()
            .transform<ext::vector<elements::html_element*>>([](elements::html_opt_group_element* element) {return element->children;})
            .flatten()
            .cast_all<elements::html_option_element*>();

    // combine the two lists
    children.extend(grandchildren);
    return children;
}


auto html::helpers::input_internals::get_placeholder_label_option(
        elements::html_select_element* select_element)
        -> elements::html_option_element*
{
    // get the first option element in the list of options for the select element
    auto* first_option_element = list_of_options(select_element).front();

    // return the first option element if the select element is required, non-multiple, display size 1, and the first
    // option element is the empty string and its parent is the select element
    if (select_element->required and not select_element->multiple and display_size(select_element) == 1 and
            first_option_element->value == "" and first_option_element->parent == select_element)
        return first_option_element;
}


auto html::helpers::input_internals::ask_for_reset(
        elements::html_option_element* option_element)
        -> void
{
    // run the selected setting algorithm for the container HTMLSelectElement ancestor (parent or grandparent)
    selectedness_setting_algorithm(select_element_parent(option_element));
}


auto html::helpers::input_internals::send_select_update_notifications(
        elements::html_select_element* select_element)
        -> void
{
    // fire composed input event at the select element, and then a change event (both bubbling)
    dom::helpers::event_dispatching::fire_event<>("input" , select_element, {{"bubbles", true}, {"composed", true}});
    dom::helpers::event_dispatching::fire_event<>("change", select_element, {{"bubbles", true}                    });
}
