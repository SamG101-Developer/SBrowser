#pragma once
#ifndef SBROWSER_CANVAS_DRAW_IMAGE_HPP
#define SBROWSER_CANVAS_DRAW_IMAGE_HPP

#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_draw_image;}
namespace html::canvas::paint {class canvas_image_source;}


template<typename T>
class html::canvas::mixins::canvas_draw_image : virtual public dom_object
{
public js_methods:
    auto draw_image(paint::canvas_image_source* image, double dx, double dy, double dw = 0, double dh = 0);
    auto draw_image(paint::canvas_image_source* image, double sx, double sy, double sw, double sh, double dx, double dy, double dw, double dh);

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_DRAW_IMAGE_HPP
