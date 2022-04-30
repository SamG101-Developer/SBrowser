#pragma once
#ifndef SBROWSER_CONTENT_EDITING_INTERNALS_HPP
#define SBROWSER_CONTENT_EDITING_INTERNALS_HPP

namespace html::helpers {struct content_editing_internals;}

#include <ext/string.hpp>

namespace html::elements {class html_element;}


struct html::helpers::content_editing_internals
{
    static auto is_editing_host(
            const elements::html_element* element)
            -> bool;

    static auto compute_autocapitalization_hint(
            elements::html_element* element)
            -> ext::string;

    static auto used_autocapitalization_hint(
            elements::html_element* element)
            -> ext::string;
};


#endif //SBROWSER_CONTENT_EDITING_INTERNALS_HPP
