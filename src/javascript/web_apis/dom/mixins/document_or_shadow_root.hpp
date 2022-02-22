#pragma once
#ifndef SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP
#define SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP

#include <ext/iterables/vector.hpp>
#include <ext/properties/dom_property.hpp>
#include <dom_object.hpp>

namespace css::cssom::style_sheets {class css_style_sheet;}

namespace dom {
    namespace mixins {template <typename T> class document_or_shadow_root;}
    namespace nodes {class element;}
}


template <typename T>
class dom::mixins::document_or_shadow_root : public dom_object {

public: constructors
    // dom
    ext::dom_property<nodes::element*, _F> active_element;

    // cssom
    ext::dom_property<ext::vector<css::cssom::style_sheets::css_style_sheet*>*, _F> style_sheets;
    ext::dom_property<ext::vector<css::cssom::style_sheets::css_style_sheet*>*, _F> adopted_style_sheets;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP
