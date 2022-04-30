#include "html_element_internals.hpp"

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/text.hpp>

#include <html/elements/html_base_element.hpp>
#include <html/elements/html_button_element.hpp>
#include <html/elements/html_details_element.hpp>
#include <html/elements/html_dlist_element.hpp>
#include <html/elements/html_element.hpp>
#include <html/elements/html_div_element.hpp>
#include <html/elements/html_li_element.hpp>
#include <html/elements/html_menu_element.hpp>
#include <html/elements/html_olist_element.hpp>
#include <html/elements/html_opt_group_element.hpp>
#include <html/elements/html_option_element.hpp>
#include <html/elements/html_select_element.hpp>
#include <html/elements/html_text_area_element.hpp>
#include <html/elements/html_ulist_element.hpp>


static ext::string_vector known_directions{"ltr", "rtl", "auto"};


auto html::helpers::html_element_internals::directionality(
        elements::html_element* element)
        -> internal::direction_mode
{
    if (element->dir == "ltr" or dom::helpers::trees::is_document_element(element))
        return internal::direction_mode::LTR;

    if (element->dir == "rtl")
        return internal::direction_mode::RTL;

    if (auto* text_area_element = dynamic_cast<elements::html_text_area_element*>(element); element->dir == "auto")
    {
        if (/* TODO */ false)
            return internal::direction_mode::RTL;

        else if (not text_area_element->value->empty() or dom::helpers::trees::is_document_element(text_area_element))
            return internal::direction_mode::LTR;

        else
            return directionality(text_area_element->parent_element);
    }

    if (element->dir == "auto" or (element->local_name == "bdi" and not known_directions.contains(element->dir)))
        ; // TODO

    if (element->parent_element and not known_directions.contains(element->dir))
        return directionality(element->parent_element);
}


auto html::helpers::html_element_internals::merge_with_next_text_node(
        dom::nodes::text* text_node)
        -> void
{
    // get the next sibling node cast to a text node
    auto* next_text_node = ext::property_dynamic_cast<dom::nodes::text*>(text_node->next_sibling);

    // if the next sibling node is a text node, then continue
    if (next_text_node)
    {
        // append the next sibling node's data to this node's data, and remove the next text node (if it has a parent)
        dom::helpers::texts::replace_data(text_node, text_node->data->length(), 0, next_text_node->data);
        if (next_text_node->parent) dom::helpers::mutation_algorithms::remove(next_text_node);
    }
}


auto html::helpers::html_element_internals::starting_value(
        elements::html_olist_element* element)
        -> int {

    // return the start value if it exists
    if (element->start)
        return element->start;

    // return the number of HTMLLIElement children is the element is reversed
    if (element->reversed)
        return element->children->cast_all<elements::html_li_element*>().length();

    // otherwise, return 1
    return 1;
}


auto html::helpers::html_element_internals::list_owner(
        elements::html_li_element* element)
        -> elements::html_element*
{
    // return null if the element is not being rendered ie has no owner
    if (not being_rendered(element))
        return nullptr;

    // get the closest ancestor that is a menu type element TODO : reverse list first?
    auto ancestor_list_elements = dom::helpers::trees::ancestors(element).filter([](dom::nodes::node* node) {
        return multi_cast<elements::html_olist_element*, elements::html_ulist_element*, elements::html_menu_element*>(node);
    });

    // if there is no such menu type ancestor, set the closest ancestor to the parent element
    dom::nodes::node* ancestor = not ancestor_list_elements.empty() ? ancestor_list_elements.front() : element->parent;

    // TODO : CSS box stuff
}


auto html::helpers::html_element_internals::name_value_group(
        elements::html_dlist_element* element)
        -> ext::vector<ext::string_string_map>
{
    // create a list of string-string maps, a string-string map, and a bool seen_dd flag
    ext::vector<ext::string_string_map> groups;
    ext::string_string_map current;
    bool seen_dd = false;

    // get the first child, and set the current grandchild to nullptr
    dom::nodes::node* child = element->child_nodes->front();
    dom::nodes::node* grandchild = nullptr;

    // iterate until all the children have been processed
    while (child)
    {
        // if the child is a div element, then process grandchildren
        if (dynamic_cast<elements::html_div_element*>(child))
        {
            // get the first grandchild, and iterate until all the grandchildren have been processed
            grandchild = child->child_nodes->front();
            while (grandchild)
            {
                // process the grandchild and move to the next grandchild
                process_dt_or_dd(grandchild, current, seen_dd);
                grandchild = grandchild->next_sibling;
            }
        }

        // otherwise, handle the child
        else
        {
            // process the child, and move to the next child
            process_dt_or_dd(child);
            child = child->next_sibling;
        }
    }

    // append the current string-string map into the groups and return the groups
    groups.append(current);
    return groups;
}


auto html::helpers::html_element_internals::process_dt_or_dd(
        dom::nodes::node* node,
        ext::vector<ext::string_string_map>& groups,
        ext::string_string_map& current,
        bool& seen_dd)
        -> void
{
    // handle 'dt' elements
    if (auto* dt_element = dynamic_cast<dom::nodes::element*>(node); dt_element->local_name == "dt")
    {
        // if a 'dd' element has already beem seen
        if (seen_dd)
        {
            // append the current to the groups, reset the current, and set seen_dd to false
            groups.append(current);
            current = {};
            seen_dd = false;
        }

        // TODO : modify 'current'
    }

    // handle 'dd' elements
    else if (auto* dd_element = dynamic_cast<dom::nodes::element*>(node); dd_element->local_name == "dd")
    {
        // TODO : modify 'current'
        seen_dd = true;
    }
}


auto html::helpers::html_element_internals::details_notification_task_steps(
        elements::html_details_element* details_element)
        -> void
{
    // TODO : check if already queued

    dom::helpers::event_dispatching::fire_event<>("toggle", details_element);
}


auto html::helpers::html_element_internals::ancestor_details_revealing_algorithm(
        elements::html_details_element* details_element)
        -> void
{
    auto flat_tree = dom::helpers::trees::descendants(dom::helpers::trees::root(details_element));

    while (flat_tree.contains(details_element->parent))
    {
        // TODO
    }
}


auto html::helpers::html_element_internals::is_summary_for_parent_details(
        elements::html_element* summary_element)
        -> bool
{
    // return false if the summary element has no parent
    if (not summary_element->parent)
        return false;

    // get the parent element as a HTMLDetailsElement element
    auto* details_parent_element = dynamic_cast<elements::html_details_element*>(summary_element);

    // return false if the parent isn't a HTMLDetailsElement
    if (not details_parent_element)
        return false;

    // return false if the parent's first summary element child isn't summary_element
    if (details_parent_element->children->filter([](dom::nodes::element* element) {return element->local_name == "summary";}).front() != summary_element)
        return false;

    // return true if all the other conditions have passed
    return true;
}


auto html::helpers::html_element_internals::is_actually_disabled(
        elements::html_element* element)
        -> bool
{
    if (auto* button = dynamic_cast<elements::html_button_element*>(element))
        return button->disabled;

    if (auto* select = dynamic_cast<elements::html_select_element*>(element))
        return select->disabled;

    if (auto* textarea = dynamic_cast<elements::html_text_area_element*>(element))
        return textarea->disabled;

    if (auto* optgroup = dynamic_cast<elements::html_opt_group_element*>(element))
        return optgroup->disabled;

    if (auto* option = dynamic_cast<elements::html_option_element*>(element))
        return option->disabled;

    if (auto* form_associated = dynamic_cast<mixins::form_associated<elements::html_element>*>(element))
        return form_associated->disabled;
}


auto html::helpers::html_element_internals::ancestor_hidden_until_found_revealing_algorithm(
        elements::html_element* element)
        -> void
{
    do
    {
        if (element->hidden)
        {
            dom::helpers::event_dispatching::fire_event<>("beforematch", element);

            JS_BLOCK_START
            auto v8_isolate = v8::Isolate::GetCurrent();
            auto v8_attribute = v8pp::convert<ext::string>::to_v8(v8_isolate, "hidden");
            auto v8_object = v8pp::convert<elements::html_element*>::to_v8(v8_isolate, element);
            v8_object->Delete(v8_isolate->GetCurrentContext(), v8_attribute);
            JS_BLOCK_END
        }
    } while ((element = element->parent));
}
