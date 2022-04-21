#include "image_bitmap_rendering_context.hpp"

#include <dom/helpers/exceptions.hpp>
#include <html/helpers/canvas_internals.hpp>


auto html::canvas::image_bitmap_rendering_context::transfer_from_image_bitmap(
        paint::image_bitmap* bitmap)
        -> void
{
    if (not bitmap)
    {
        helpers::canvas_internals::set_image_bitmap_rendering_contexts_output_bitmap(this, nullptr);
        return;
    }

    dom::helpers::exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "Detached slot must be false",
            [this] {return not s_detached;});

    helpers::canvas_internals::set_image_bitmap_rendering_contexts_output_bitmap(this, bitmap);
    s_detached = true;

    bitmap = nullptr;
}


auto html::canvas::image_bitmap_rendering_context::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<image_bitmap_rendering_context>{isolate}
            .inherit<abstract_rendering_context>()
            .function("transferFromImageBitmap", &image_bitmap_rendering_context::transfer_from_image_bitmap)
            .var("canvas", &image_bitmap_rendering_context::canvas)
            .auto_wrap_objects();
}
