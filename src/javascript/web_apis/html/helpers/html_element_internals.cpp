#include "html_element_internals.hpp"

#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/text.hpp>

#include <html/elements/html_base_element.hpp>
#include <html/elements/html_element.hpp>


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


template <typename T>
auto html::helpers::html_element_internals::get_text_elements_target(
        mixins::targetable<T>* element)
        -> ext::string
{
    // if the element has a target, then return the target attribute
    if (not element->target->empty())
        return element->target;

    // otherwise, if there is a HTMLBaseElement in the document, return its target attribute
    else if (auto* base_element = static_cast<T*>(element)->owner_document->child_nodes->template cast_all<elements::html_base_elememt*>().front())
        return base_element->target;

    // otherwise, there is no target, so return the empty string
    return "";
}
