#pragma once
#ifndef SBROWSER_HEADER_INTERNALS_HPP
#define SBROWSER_HEADER_INTERNALS_HPP

namespace fetch::helpers {struct header_internals;}

#include <ext/optional.hpp>
#include <ext/vector.hpp>
#include <ext/string.hpp>


struct fetch::helpers::header_internals
{
    using header_t = std::pair<ext::string, ext::string>;
    using header_list_t = ext::vector<header_t>;
    using header_name_t = ext::string;
    using header_value_t = ext::string;

    static auto get_structured_field(
            const header_name_t& name,
            const header_value_t& type,
            const header_list_t& list)
            -> ext::optional<header_name_t>;

    static auto set_structured_field(
            const header_t& header,
            header_list_t& list)
            -> void;

    static auto contains_header(
            const header_name_t& name,
            const header_list_t& list)
            -> bool;

    static auto get(
            const header_name_t& name,
            const header_list_t& list)
            -> ext::optional<header_value_t>;

    static auto get_decode_split(
            const header_name_t& name,
            const header_list_t& list)
            -> ext::vector<header_value_t>;

    static auto append(
            const header_t& header,
            header_list_t& list)
            -> void;

    static auto delete_(
            const header_name_t& name,
            header_list_t list)
            -> void;

    static auto set(
            const header_t& header,
            header_list_t& list)
            -> void;

    static auto combine(
            const header_t& header,
            header_list_t& list)
            -> void;

    static auto convert_header_names_to_sorted_lowercase_set(
            const ext::vector<header_name_t>& header_names)
            -> ext::vector<header_name_t>;

    static auto sort_and_combine(
            const header_list_t& list)
            -> header_list_t;

    static auto normalize(
            const header_value_t& potential_value)
            -> header_name_t;

    static auto is_cors_safelisted_request_header(
            const header_t& header)
            -> bool;

    static auto is_cors_unsafe_request_header_byte(
            const std::byte& byte)
            -> bool;

    static auto cors_unsafe_request_header_list()
            -> ext::vector<header_name_t>;

    static auto is_cors_non_wildcard_request_header_name(
            const header_name_t& name)
            -> bool;

    static auto is_priveleged_no_cors_request_header_name(
            const header_name_t& name)
            -> bool;

    static auto is_cors_safelisted_response_header_name(
            const header_name_t& name)
            -> bool;

    static auto is_no_cors_safelisted_request_header_name(
            const header_name_t& name)
            -> bool;

    static auto is_no_cors_safelisted_request_header(
            const header_t& header)
            -> bool;

    static auto is_forbidden_header_name(
            const header_name_t& name)
            -> bool;

    static auto is_forbidden_reponse_header_name(
            const header_name_t& name)
            -> bool;

    static auto is_request_body_header_name(
            const header_name_t& name)
            -> bool;

    static auto extract_header_values(
            const header_t& header)
            -> ext::vector<header_value_t>;

    static auto extract_header_list_values(
            const header_name_t& name,
            const header_list_t& list)
            -> ext::vector<header_value_t>;

    static auto is_simple_range_header(
            const header_value_t)
            -> bool;

    static auto parse_structured_fields(
            const ext::string& input_string,
            const ext::string& header_type)
            -> ext::string;
};


#endif //SBROWSER_HEADER_INTERNALS_HPP
