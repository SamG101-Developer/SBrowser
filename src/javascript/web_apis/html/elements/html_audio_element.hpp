#pragma once
#ifndef SBROWSER_HTML_AUDIO_ELEMENT_HPP
#define SBROWSER_HTML_AUDIO_ELEMENT_HPP

#include <html/elements/html_media_element.hpp>

namespace html::elements {class html_audio_element;}


class html::elements::html_audio_element : public html_media_element
{
public constructors:
    html_audio_element();

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_AUDIO_ELEMENT_HPP
