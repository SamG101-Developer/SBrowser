#pragma once
#ifndef SBROWSER_CANVAS_IMAGE_DATA_HPP
#define SBROWSER_CANVAS_IMAGE_DATA_HPP

#include <dom_object.hpp>
#include <ext/map.hpp>

namespace html::canvas::mixins {template<typename T> class canvas_image_data;}
namespace html::canvas::paint {class image_data;}


template <typename T>
class html::canvas::mixins::canvas_image_data : public virtual dom_object
{
public js_methods:
    new_obj auto get_image_data(enforce_range long sx, enforce_range long sy, enforce_range long sw, enforce_range long sh, const ext::string_any_map& settings = {}) -> paint::image_data;
    new_obj auto create_image_data(enforce_range long sw, enforce_range long sh, const ext::string_any_map& settings = {}) -> paint::image_data;
    new_obj auto create_image_data(paint::image_data* image_data) -> paint::image_data;
    auto put_image_data(paint::image_data* image_data, enforce_range long dx, enforce_range long dy) -> void;
    auto put_image_data(paint::image_data* image_data, enforce_range long dx, enforce_range long dy, enforce_range long dirty_x, enforce_range long dirty_y, enforce_range long dirty_width, enforce_range long dirty_height) -> void;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_IMAGE_DATA_HPP
