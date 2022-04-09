#pragma once
#ifndef SBROWSER_INPUT_INTERNALS_HPP
#define SBROWSER_INPUT_INTERNALS_HPP

#include <ext/vector.hpp>

namespace html::elements {class html_option_element;}
namespace html::elements {class html_select_element;}
namespace html::helpers {struct input_internals;}


struct html::helpers::input_internals
{
    static auto display_size(
            elements::html_select_element* select_element)
            -> int;

    static auto list_of_options(
            elements::html_select_element* select_element)
            -> ext::vector<elements::html_option_element*>;

    static auto get_placeholder_label_option(
            elements::html_select_element* select_element)
            -> elements::html_option_element*;

    static auto selectedness_setting_algorithm(
            elements::html_select_element* select_element)
            -> void;

    static auto ask_for_reset(
            elements::html_option_element* option_element)
            -> void;

    static auto send_select_update_notifications(
            elements::html_select_element* select_element)
            -> void;

    static auto select_element_parent(
            elements::html_option_element* option_element)
            -> elements::html_select_element*
};


#endif //SBROWSER_INPUT_INTERNALS_HPP
