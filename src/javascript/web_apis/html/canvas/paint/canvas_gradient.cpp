#include "canvas_gradient.hpp"

#include <dom/helpers/exceptions.hpp>


auto html::canvas::paint::canvas_gradient::add_color_stop(
        const double offset,
        const ext::string& color)
        -> void
{
    // if the offset is less than 0 or greater than 1, throw an index size error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "Offset must be between 0.0 and 1.0",
            [offset] {return offset < 0 or offset > 1;});

    // set the color at the offset (creating a QColor object)
    m_gradient->setColorAt(offset, QColor(color));
}


html::canvas::paint::canvas_gradient::operator QGradient() const
{
    return *m_gradient;
}


auto html::canvas::paint::canvas_gradient::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_gradient>{isolate}
            .inherit<dom_object>()
            .function("addColorStop", &canvas_gradient::add_color_stop)
            .auto_wrap_objects();
}
