#pragma once
#ifndef SBROWSER_HTML_IMAGE_ELEMENT_HPP
#define SBROWSER_HTML_IMAGE_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/lazy_loading.hpp>
#include <web_idl/types/promise.hpp>

#include <QtWidgets/QLabel>

namespace html::elements {class html_image_element;}
namespace html::internal {struct image_dimensions;}
namespace html::internal {struct image_request;}


class html::elements::html_image_element
        : public html_element
        , public mixins::lazy_loading
{
public constructors:
    html_image_element();

public methods:
    auto decode() -> webidl::types::promise<void>;

public properties:
    ext::html_property<ext::string, _T> alt;
    ext::html_property<ext::string, _T> src;
    ext::html_property<ext::string, _T> srcset;
    ext::html_property<ext::string, _T> sizes;
    ext::html_property<ext::string, _T> cross_origin;
    ext::html_property<ext::string, _T> use_map;
    ext::html_property<ext::string, _T> referrer_policy;
    ext::html_property<ext::string, _T> decoding;
    ext::html_property<ext::string> current_src;

    ext::html_property<unsigned long, _T> width;
    ext::html_property<unsigned long, _T> height;
    ext::html_property<unsigned long> natural_width;
    ext::html_property<unsigned long> natural_height;

    ext::html_property<bool, _T> is_map;
    ext::html_property<bool> complete;

public internal_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
    auto render() const -> QLabel* override;

private accessors:
    auto get_width() const -> unsigned long;
    auto get_height() const -> unsigned long;
    auto get_natural_width() const -> unsigned long;
    auto get_natural_height() const -> unsigned long;
    auto get_complete() -> bool;

    auto set_alt(const ext::string& val) -> void;
    auto set_src(const ext::string& val) -> void;
    auto set_width(unsigned long val) -> void;
    auto set_height(unsigned long val) -> void;

private internal_properties:
    html_image_element* m_dimension_attribute_src;
    internal::image_request* m_current_request = nullptr;
    internal::image_request* m_pending_request = nullptr;
    html_image_element* m_last_selected_source = nullptr;
    int m_current_pixel_density = 1;
    internal::image_dimensions* m_preferred_density_corrected_dimensions = nullptr;
};


#endif //SBROWSER_HTML_IMAGE_ELEMENT_HPP
