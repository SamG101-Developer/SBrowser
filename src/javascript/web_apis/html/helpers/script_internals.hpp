#pragma once
#ifndef SBROWSER_SCRIPT_INTERNALS_HPP
#define SBROWSER_SCRIPT_INTERNALS_HPP

namespace html::helpers {struct script_internals;}
namespace html::internal {struct script_fetch_options;}
namespace html::internal {struct script;}
namespace html::internal {struct classic_script;}
namespace html::internal {struct module_script;}
namespace html::internal {using css_module_script = module_script;}
namespace html::internal {using json_module_script = module_script;}

#include <ext/set.hpp>
#include <ext/string.hpp>
#include <javascript/environment/realms.hpp>

namespace dom::other {class dom_exception;}
namespace html::elements {struct html_script_element;}


class html::helpers::script_internals
{
    using visiting_set_t = ext::set<std::pair<const url::url&, const ext::string&>>;

    static auto prepare_script(
            elements::html_script_element* script_element)
            -> void;

    static auto is_scripting_disabled(
            elements::html_script_element* script_element)
            -> bool;

    static auto is_parser_inserted(
            elements::html_script_element* script_element)
            -> bool;

    static auto execute_script_block(
            elements::html_script_element* script_element)
            -> void;

    // fetching
    static auto fetch_classic_script(
            url::url& url_record,
            javascript::realms::settings_object_t& settings,
            const ext::string& cors_setting,
            const ext::string& encoding)
            -> ext::optional<internal::classic_script&>;

    static auto fetch_classic_worker_script(
            url::url& url_record,
            // TODO
            const ext::string& destination,
            javascript::realms::settings_object_t& settings)
            -> ext::optional<internal::classic_script&>;

    static auto fetch_classic_worker_imported_script(
            url::url& url_record,
            javascript::realms::settings_object_t& settings)
            -> ext::optional<internal::classic_script&>;

    static auto fetch_external_module_graph_script(
            url::url& url,
            javascript::realms::settings_object_t& settings,
            const ext::string_any_map& options)
            -> ext::optional<internal::module_script&>;

    static auto fetch_import_module_script_graph(
            v8::ModuleRequest& module_request,
            url::url& url_record,
            javascript::realms::settings_object_t& settings,
            const ext::string_any_map& options)
            -> ext::optional<internal::module_script&>;

    static auto fetch_modulepreload_script_graph(
            url::url& url_record,
            const ext::string& destination,
            javascript::realms::settings_object_t& settings,
            const ext::string_any_map& options)
            -> ext::optional<internal::module_script&>;

    static auto fetch_inline_script_graph(
            const ext::string& source,
            url::url& url_record,
            javascript::realms::settings_object_t& settings,
            const ext::string_any_map& options)
            -> ext::optional<internal::module_script&>;

    static auto fetch_module_worker_script_graph(
            url::url& url_record,
            // TODO
            const ext::string& desination,
            const ext::string& credentials_mode,
            const ext::string& module_map_settings_object) // TODO
            -> ext::optional<internal::module_script&>;

    static auto fetch_worklet_script_graph(
            url::url& url_record,
            // TODO
            const ext::string& destination,
            const ext::string& credentials_mode,
            const ext::string& module_map_settings_object,
            const ext::string& module_responses_map)
            -> ext::optional<internal::module_script&>;

    static auto fetch_worklet_or_module(
            url::url& url_record,
            // TODO
            const ext::string& destination,
            const ext::string& credentials_mode,
            const ext::string& module_map_settings_object)
            -> ext::optional<internal::module_script>;

    static auto fetch_descendants_of_and_link_module_script(
            internal::module_script& module_script,
            // TODO
            const ext::string& destination,
            const visiting_set_t& visited_set)
            -> ext::optional<internal::module_script&>;

    static auto fetch_descendants_of_script(
            internal::module_script& module_script,
            // TODO
            const ext::string& destination,
            const visiting_set_t& visited_set)
            -> ext::optional<internal::module_script&>;

    static auto fetch_single_module_script(
            url::url& url_record,
            // TODO
            const ext::string& destination,
            const ext::string_any_map& options,
            const ext::string& module_map_settings_object,
            const ext::string& referrer,
            v8::ModuleRequest module_request,
            bool top_level_module_fetch_flag)
            -> ext::optional<internal::module_script&>;

    static auto internal_module_script_graph_fetching_procedure(
            v8::ModuleRequest& module_request,
            // TODO
            const ext::string& destination,
            const ext::string_any_map& options,
            const ext::string& module_map_settings_object,
            const visiting_set_t& visited_set,
            const ext::string& referrer)
            -> ext::optional<internal::module_script&>;

    static auto find_first_parse_error(
            internal::module_script& module_script,
            const visiting_set_t& discovered_set = {})
            -> dom::other::dom_exception&

    // creating
    static auto create_classic_script(
            const ext::string& source,
            javascript::realms::settings_object_t& settings,
            const url::url& base_url,
            const ext::string_any_map& options,
            bool muted_errors = false)
            -> void;

    static auto create_module_script(
            const ext::string& script,
            javascript::realms::settings_object_t& settings,
            const url::url& base_url,
            const ext::string_any_map& options)
            -> void;

    static auto create_css_module_script(
            const ext::string& source,
            javascript::realms::settings_object_t& settings)
            -> void;

    static auto create_json_module_script(
            const ext::string& source,
            javascript::realms::settings_object_t& settings)
            -> void;

    static auto module_type_from_module_request(
            v8::ModuleRequest& module_request)
            -> void;

    static auto module_type_allowed(
            const ext::string& module_type,
            javascript::realms::settings_object_t& settings)
            -> bool;

    static auto active_script()
            -> void;
};


struct html::internal::script_fetch_options
{
    ext::string cryptographic_nonce;
    ext::string integrity_metadata;
    ext::string parser_metadata{"not-parser-inserted"};
    ext::string credentials_mode;
    ext::string referrer_policy;
};


struct html::internal::script
{
    javascript::realms::settings_object_t& settings;
    ext::string record;
    ext::string parse_error;
    ext::string error_to_rethrow;
    ext::string_any_map script_fetch_options;
    url::url& base_url;
};


struct html::internal::classic_script : public script
{
    bool muted_errors;
};


struct html::internal::module_script : public script {};


#endif //SBROWSER_SCRIPT_INTERNALS_HPP
