#pragma once
#ifndef SBROWSER_IMAGE_DATA_HPP
#define SBROWSER_IMAGE_DATA_HPP

#include <ext/map.hpp>
#include <dom_object.hpp>

#include <render/painting/image.hpp>

namespace html::canvas::paint {class image_data;}


class html::canvas::paint::image_data : public virtual dom_object
{
public constructors:
    image_data(unsigned long sw, unsigned long sh, const ext::string_any_map_t& settings = {});
    image_data(ext::uint8_array data, unsigned long sw, unsigned long sh, const ext::string_any_map_t& settings = {});

public js_properties:
    ext::html_property<unsigned long, _F> width;
    ext::html_property<unsigned long, _F> height;
    ext::html_property<ext::uint8_array*, _F> data;
    ext::html_property<ext::string, _F> color_space;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    render::painting::image m_image;
};


#endif //SBROWSER_IMAGE_DATA_HPP
