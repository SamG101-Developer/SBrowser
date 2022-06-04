#include "html_audio_element.hpp"


html::elements::html_audio_element::html_audio_element()
        : html_media_element{}
{
    // set the attribute's values
    m_rendered_widget = nullptr;
}


auto html::elements::html_audio_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_audio_element>{isolate}
            .ctor<>()
            .inherit<html_media_element>()
            .auto_wrap_objects();
}
