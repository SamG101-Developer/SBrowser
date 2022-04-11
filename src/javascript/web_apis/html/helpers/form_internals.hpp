#pragma once
#ifndef SBROWSER_FORM_INTERNALS_HPP
#define SBROWSER_FORM_INTERNALS_HPP

#include <ext/string.hpp>

namespace dom::nodes {class element;}
namespace html::elements {class html_element;}
namespace html::elements {class html_form_element;}
namespace html::helpers {struct form_internals;}
namespace html::mixins {template<typename T> class form_associated;}


struct html::helpers::form_internals
{
    using entry_t = std::tuple<ext::string, ext::string>;

    static auto supported_property_names(
            elements::html_form_element* form_element)
            -> ext::string_vector;

    static auto determine_value_of_named_property(
            elements::html_form_element* form_element)
            -> dom::nodes::element*;

    static auto pick_encoding_for_form(
            elements::html_form_element* form_element)
            -> ext::string;

    static auto convert_list_of_name_value_pairs(
            ext::vector<entry_t>& list)
            -> ext::vector<entry_t>;

    static auto reset(
            elements::html_form_element* form_element)
            -> void;

    template<typename T>
    static auto submit(
            elements::html_form_element* form_element,
            mixins::form_associated<T>* submitter,
            bool submitted_from_submit_flag = false)
            -> void;

    template<typename T>
    static auto reset_form_owner(
            mixins::form_associated<T>* form_associated_element)
            -> void;

    template<typename T>
    static auto statically_validate_constraints(
            mixins::form_associated<T>* form_associated_element)
            -> short;

    template<typename T>
    static auto interactively_validate_constraints(
            mixins::form_associated<T>* form_associated_element)
            -> short;

    static auto no_validate_state(
            elements::html_element* element)
            -> bool;

    static auto create_an_entry(
            const ext::string& name,
            const ext::string& value,
            const ext::string& file_string)
            -> entry_t;

    template <typename T>
    static auto construct_entry_list(
            elements::html_form_element* form_element,
            mixins::form_associated<T>* form_associated_element = nullptr,
            const ext::string& encoding = "")
            -> ext::vector<entry_t>;
};


#endif //SBROWSER_FORM_INTERNALS_HPP
