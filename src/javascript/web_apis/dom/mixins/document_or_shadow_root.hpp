#pragma once
#ifndef SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP
#define SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP

#include <dom_object.hpp>
namespace dom::mixins {template <typename T> class document_or_shadow_root;}

#include <ext/vector.hpp>
namespace css::cssom::style_sheets {class css_style_sheet;}
namespace dom::nodes {class element;}


template <typename T>
class dom::mixins::document_or_shadow_root : public virtual dom_object
{
public constructors:
    document_or_shadow_root();

public js_properties:
    // html
    ext::property<smart_pointer<nodes::element>> active_element;

    // cssom
    ext::property<smart_pointer<ext::vector<smart_pointer<css::cssom::style_sheets::css_style_sheet>>>> style_sheets;
    ext::property<smart_pointer<ext::vector<smart_pointer<css::cssom::style_sheets::css_style_sheet>>>> adopted_style_sheets;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    [[nodiscard]] auto get_active_element() const -> smart_pointer<nodes::element>;
};


#endif //SBROWSER_DOCUMENT_OR_SHADOW_ROOT_HPP
