#pragma once
#ifndef SBROWSER_FORM_INTERNALS_HPP
#define SBROWSER_FORM_INTERNALS_HPP

#include <ext/string.hpp>

namespace dom::nodes {class element;}
namespace html::elements {class html_form_element;}
namespace html::helpers {struct form_internals;}


struct html::helpers::form_internals
{
    static auto supported_property_names(
            elements::html_form_element* form_element)
            -> ext::string_vector;

    static auto determine_value_of_named_property(
            elements::html_form_element* form_element)
            -> dom::nodes::element*;
};


#endif //SBROWSER_FORM_INTERNALS_HPP
