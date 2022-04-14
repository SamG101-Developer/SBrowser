#include "image_data.hpp"


auto html::canvas::paint::image_data::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    #define CTOR_T_1 unsigned long, unsigned long, const ext::string_any_map&
    #define CTOR_T_2 ext::uint8_array, unsigned long, unsigned long, const ext::string_any_map&

    return v8pp::class_<image_data>{isolate}
            .ctor<CTOR_T_1>()
            .ctor<CTOR_T_2>()
            .inherit<dom_object>()
            .var("width", &image_data::width)
            .var("height", &image_data::height)
            .var("data", &image_data::data)
            .var("colorSpace", &image_data::color_space)
            .auto_wrap_objects();

    #undef CTOR_T_1
    #undef CTOR_T_2
}
