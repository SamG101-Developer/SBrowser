#include "html_element_internals.hpp"

#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/text.hpp>

#include <html/elements/html_base_element.hpp>
#include <html/elements/html_dlist_element.hpp>
#include <html/elements/html_element.hpp>
#include <html/elements/html_div_element.hpp>
#include <html/elements/html_li_element.hpp>
#include <html/elements/html_menu_element.hpp>
#include <html/elements/html_olist_element.hpp>
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

    if (element->dir == "auto" and (auto* text_area_element = dynamic_cast<elements::html_text_area_element*>(element)))
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
