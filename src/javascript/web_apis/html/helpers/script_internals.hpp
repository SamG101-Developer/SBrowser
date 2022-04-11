#pragma once
#ifndef SBROWSER_SCRIPT_INTERNALS_HPP
#define SBROWSER_SCRIPT_INTERNALS_HPP

#include <ext/string.hpp>

namespace html::elements {struct html_script_element;}
namespace html::helpers {struct script_internals;}


class html::helpers::script_internals
{
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

    struct script_fetch_options
    {
        ext::string cryptographic_nonce;
        ext::string integrity_metadata;
        ext::string parser_metadata;
        ext::string credentials_mode;
        ext::string referrer_policy;
    };
};


#endif //SBROWSER_SCRIPT_INTERNALS_HPP
