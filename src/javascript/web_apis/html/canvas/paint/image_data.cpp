#include "image_data.hpp"

#include <dom/helpers/exceptions.hpp>
#include <html/helpers/image_internals.hpp>


html::canvas::paint::image_data::image_data(
        const unsigned long sw,
        const unsigned long sh,
        const ext::string_any_map& settings)
{
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "width or height can not be 0",
            [sw, sh] {return sw == 0 or sh == 0;});

    helpers::image_internals::initialize_image(this, sw, sh, settings);
    m_image.set_width(static_cast<int>(sw));
    m_image.set_height(static_cast<int>(sh));
    m_image.reset();
}


html::canvas::paint::image_data::image_data(
        ext::uint8_array data,
        const unsigned long sw,
        const unsigned long sh,
        const ext::string_any_map& settings)
{
    auto length = data.length();
    dom::helpers::exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "length must be a multiple of 4",
            [length] {return length % 4 == 0;});

    length /=4 ;
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "length must be a multiple of the width",
            [length, sw] {return length % sw == 0;});

    auto calculated_sh = length / sw;
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "invalid height specified", // TODO : improve message
            [calculated_sh, sh] {return calculated_sh != sh;});

    helpers::image_internals::initialize_image(this, sw, sh, settings, &data);
    m_image.set_width(static_cast<int>(sw));
    m_image.set_height(static_cast<int>(sh));
}


//html::canvas::paint::image_data::cr


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
