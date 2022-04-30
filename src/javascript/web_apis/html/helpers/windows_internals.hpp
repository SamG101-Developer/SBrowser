#pragma once
#ifndef SBROWSER_WINDOWS_INTERNALS_HPP
#define SBROWSER_WINDOWS_INTERNALS_HPP

namespace html::helpers {struct windows_internals;}

#include <ext/string.hpp>
#include <ext/map.hpp>


struct html::helpers::windows_internals
{
    static inline ext::string_vector feature_separators {"=", ",", " "};

    static auto window_open_steps(
            const ext::string& url,
            const ext::string& target,
            const ext::string& features)
            -> void;

    static auto tokenize_features(
            const ext::string& features)
            -> ext::string_string_map;

    static auto check_if_window_feature_set(
            const ext::string_string_map& tokenized_features,
            const ext::string& feature_name,
            bool default_value)
            -> bool;

    static auto check_if_popup_window_requested(
            ext::string_string_map& tokenized_features)
            -> bool;

    static auto parse_boolean_feature(
            const ext::string& value)
            -> bool;
};


#endif //SBROWSER_WINDOWS_INTERNALS_HPP
