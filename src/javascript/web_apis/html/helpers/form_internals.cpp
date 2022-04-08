#include "form_internals.hpp"

#include <tuple>

#include <html/elements/html_form_element.hpp>


auto html::helpers::form_internals::supported_property_names(
        elements::html_form_element* form_element)
        -> ext::string_vector
{
    ext::vector<std::tuple<ext::string, dom::nodes::element*, ext::string, int>> sources_names;
    // TODO
}
