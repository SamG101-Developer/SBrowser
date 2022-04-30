#include "windows_internals.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/window.hpp>

#include <html/helpers/browsing_context_internals.hpp>


auto html::helpers::windows_internals::window_open_steps(
        const ext::string& url,
        const ext::string& target,
        const ext::string& features)
        -> void
{
    // TODO : event loop stuff

    auto tokenized_features = tokenize_features(features);

    auto noreferrer = tokenized_features.has_key("noreferrer") and parse_boolean_feature(tokenized_features.at("noreferrer"));
    auto noopener   = tokenized_features.has_key("noopener")   and parse_boolean_feature(tokenized_features.at("noopener")) or noreferrer;

    auto* source_browsing_context = javascript::realms::entry_realm().global_object->document->m_browsing_context;
    auto [target_browsing_context, window_type] = browsing_context_internals::choose_browsing_context(target, source_browsing_context, noopener);

    if (not target_browsing_context)
        return;

    if (not ext::string_vector{"new and unrestricted", "new with no opener"}.contains(window_type))
    {
        target_browsing_context->is_popup = check_if_popup_window_requested(tokenized_features);
        // TODO : CSSOM

        ext::string url_record = "about:blank";
        // TODO : url stuff
        // TODO : fetching
    }
    else
    {
        // TODO
    }
}


auto html::helpers::windows_internals::tokenize_features(
        const ext::string& features)
        -> ext::string_string_map
{
    ext::string_string_map tokenized_features;
    auto position = features.begin();

    while (position != tokenized_features.end())
    {
        ext::string name, value;
        while (feature_separators.contains(&*position++)); // TODO : um?
        while (not feature_separators.contains(&*position++)) name += &*position;

        for (position++; position != tokenized_features.end() and *position != '=';)
        {
            if (*position == '=' or not feature_separators.contains(&*position)) break;
        }

        if (feature_separators.contains(&*position))
        {
            for (position++; position != tokenized_features.end() and feature_separators.contains(&*position);)
            {
                if (*position == ',') break;
            }

            while (not feature_separators.contains(&*position++)) value += &*position;
        }

        if (name) tokenized_features.insert(name, value);
    }

    return tokenized_features;
}


auto html::helpers::windows_internals::check_if_window_feature_set(
        const ext::string_string_map& tokenized_features,
        const ext::string& feature_name,
        const bool default_value)
        -> bool
{
    return tokenized_features.has_key("featureName")
            ? parse_boolean_feature(tokenized_features.at("featureName"))
            : default_value;
}


auto html::helpers::windows_internals::check_if_popup_window_requested(
        ext::string_string_map& tokenized_features)
        -> bool
{
    if (tokenized_features.empty())
        return false;

    return tokenized_features.has_key("popup") and parse_boolean_feature(tokenized_features.at("popup"))
            or not check_if_window_feature_set(tokenized_features, "location", false) and not check_if_window_feature_set(tokenized_features, "toolbar", false)
            or not check_if_window_feature_set(tokenized_features, "menubar", false)
            or not check_if_window_feature_set(tokenized_features, "resizable", true)
            or not check_if_window_feature_set(tokenized_features, "scrollbars", false)
            or not check_if_window_feature_set(tokenized_features, "status", false);
}


auto html::helpers::windows_internals::parse_boolean_feature(
        const ext::string& value)
        -> bool
{
    if (ext::string_vector{"", "yes", "true"}.contains(value))
        return true;

    int parsed;
    TRY
        parsed = std::stoi(static_cast<std::string>(value));
    CATCH
        parsed = 0;
    END_TRY
    return parsed;
}
