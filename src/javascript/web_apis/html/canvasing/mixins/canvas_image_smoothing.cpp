#include "canvas_image_smoothing.hpp"


template<typename T>
html::canvasing::mixins::canvas_image_smoothing<T>::canvas_image_smoothing()
{
    // constrain the property values
    image_smoothing_quality.constrain_values({"low", "medium", "high"});

    // set the properties
    image_smoothing_enabled << true;
    image_smoothing_quality << "low";
}


template<typename T>
auto html::canvasing::mixins::canvas_image_smoothing<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_image_smoothing<T>>{isolate}
            .template inherit<dom_object>()
            .template var("imageSmoothingEnabled", &canvas_image_smoothing<T>::image_smoothing_enabled)
            .template var("imageSmoothingQuality", &canvas_image_smoothing<T>::image_smoothing_quality)
            .auto_wrap_objects();
}
