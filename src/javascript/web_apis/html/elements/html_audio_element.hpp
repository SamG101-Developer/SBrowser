#pragma once
#ifndef SBROWSER_HTML_AUDIO_ELEMENT_HPP
#define SBROWSER_HTML_AUDIO_ELEMENT_HPP

#include <html/elements/html_media_element.hpp>

namespace html::elements {class html_audio_element;}


class html::elements::html_audio_element : public html_media_element
{
public:
    html_audio_element();
};


#endif //SBROWSER_HTML_AUDIO_ELEMENT_HPP
