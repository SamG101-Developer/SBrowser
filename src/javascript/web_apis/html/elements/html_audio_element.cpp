#include "html_audio_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_audio_element::html_audio_element()
        : html_media_element{}
{
    // set the attributes
    m_rendered_widget = nullptr;

    local_name = "audio";
    HTML_CONSTRUCTOR
}


auto html::elements::html_audio_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_audio_element>{isolate}
            .template ctor<>()
            .template inherit<html_media_element>()
            .auto_wrap_objects();
}
