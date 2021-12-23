#pragma once
#ifndef SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP
#define SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP

#include <ext/property.hpp>

namespace css::cssom::style_sheets {class css_style_sheet;}

namespace dom {
    namespace mixins {template <typename T> class document_or_shadow_root;}
    namespace nodes {class element;}
}


template <typename T>
class dom::mixins::document_or_shadow_root {
public:
    // dom
    ext::dom_property<nodes::element*> active_element;

    // cssom
    ext::dom_property<ext::vector<css::cssom::style_sheets::css_style_sheet*>*> style_sheets;
    ext::dom_property<ext::vector<css::cssom::style_sheets::css_style_sheet*>*> adopted_style_sheets;
};


#endif //SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP
