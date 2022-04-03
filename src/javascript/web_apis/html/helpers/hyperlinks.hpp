#pragma once
#ifndef SBROWSER_HYPERLINKS_HPP
#define SBROWSER_HYPERLINKS_HPP

#include <ext/string.hpp>
#include <html/helpers/document_internals.hpp>

namespace html::helpers {struct hyperlinks;}
namespace html::mixins {template <typename T> class targetable;}
namespace html::elements {class html_element;}


struct html::helpers::hyperlinks
{
    static auto cannot_navigate(
            elements::html_element* element)
            -> bool;

    template <typename T>
    static auto get_an_elements_target(
            mixins::targetable<T>* element)
            -> ext::string;

    template <typename T>
    static auto get_an_elements_noopener(
            mixins::targetable<T>* element,
            const ext::string& target)
            -> bool;

    template <typename T>
    static auto follow_hyperlink(
            mixins::targetable<T>* element,
            const ext::string& hyperlink_suffix = "")
            -> void;

    template <typename T>
    static auto allowed_to_download(
            internal::browsing_context* initiator_browsing_context,
            internal::browsing_context* instantiator_browsing_context)
            -> bool;

    template <typename T>
    static auto download_hyperlink(
            mixins::targetable<T>* element,
            const ext::string& hyperlink_suffix = "")
            -> void;
};


#endif //SBROWSER_HYPERLINKS_HPP
