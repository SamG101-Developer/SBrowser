#pragma once
#ifndef SBROWSER_HTML_IMAGE_ELEMENT_HPP
#define SBROWSER_HTML_IMAGE_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/lazy_loading.hpp>
#include <web_idl/types/promise.hpp>

#include <QtWidgets/QLabel>

namespace html::elements {class html_image_element;}
namespace html::helpers {struct images;}
namespace html::internal {struct image_dimensions;}
namespace html::internal {struct image_request;}


class html::elements::html_image_element
        : public html_element
        , public mixins::lazy_loading
{
public friends:
    friend struct helpers::images;

public constructors:
    html_image_element();

public js_methods:
    auto decode() -> webidl::types::promise<void>;

public js_properties:
    ext::html_property<ext::string, _T> alt;
    ext::html_property<ext::string, _T> src;
    ext::html_property<ext::string, _T> srcset;
    ext::html_property<ext::string, _T> sizes;
    ext::html_property<ext::string, _T> cross_origin;
    ext::html_property<ext::string, _T> use_map;
    ext::html_property<ext::string, _T> referrer_policy;
    ext::html_property<ext::string, _T> decoding;
    ext::html_property<ext::string> current_src;

    ext::html_property<ulong, _T> width;
    ext::html_property<ulong, _T> height;
    ext::html_property<ulong, _F> natural_width;
    ext::html_property<ulong, _F> natural_height;

    ext::html_property<bool, _T> is_map;
    ext::html_property<bool, _F> complete;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
    auto qt() const -> QLabel* override;

protected cpp_methods:
    auto insertion_steps() -> void override;
    auto removal_steps(dom::nodes::node* old_parent) -> void override;

private accessors:
    auto get_width() const -> ulong;
    auto get_height() const -> ulong;
    auto get_natural_width() const -> ulong;
    auto get_natural_height() const -> ulong;
    auto get_complete() -> bool;
    auto get_current_src() -> ext::string;

    auto set_alt(const ext::string& val) -> void;
    auto set_src(const ext::string& val) -> void;
    auto set_width(ulong val) -> void;
    auto set_height(ulong val) -> void;

private cpp_properties:
    html_image_element* m_dimension_attribute_src;

    internal::image_request* m_current_request = nullptr;
    internal::image_request* m_pending_request = nullptr;

    html_image_element* m_last_selected_source = nullptr;
};


#endif //SBROWSER_HTML_IMAGE_ELEMENT_HPP
