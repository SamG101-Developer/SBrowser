#pragma once
#ifndef SBROWSER_WINDOWS_INTERNALS_HPP
#define SBROWSER_WINDOWS_INTERNALS_HPP

#include <ext/string.hpp>
#include <ext/map.hpp>

namespace html::helpers {struct windows_internals;}

namespace javascript::realms {class realm;}


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
            -> ext::string_string_map_t;

    static auto check_if_window_feature_set(
            const ext::string_string_map_t& tokenized_features,
            const ext::string& feature_name,
            bool default_value)
            -> bool;

    static auto check_if_popup_window_requested(
            ext::string_string_map_t& tokenized_features)
            -> bool;

    static auto parse_boolean_feature(
            const ext::string& value)
            -> bool;

    static auto setup_window_environment_settings_object(
            const url::url& creation_url,
            javascript::realms::realm& realm,
            const ext::string& reserved_environment,
            const url::url& top_level_creation_url,
            const ext::string& top_level_origin)
            -> void;
};


#endif //SBROWSER_WINDOWS_INTERNALS_HPP
