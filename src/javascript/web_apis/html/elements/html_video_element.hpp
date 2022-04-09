#pragma once
#ifndef SBROWSER_HTML_VIDEO_ELEMENT_HPP
#define SBROWSER_HTML_VIDEO_ELEMENT_HPP

#include <html/elements/html_media_element.hpp>

#include <QtMultimediaWidgets/QVideoWidget>

namespace html::elements {class html_video_element;}


class html::elements::html_video_element : public html_media_element
{
public constructors:
    html_video_element();

public js_properties:
    ext::html_property<ulong, _T> width;
    ext::html_property<ulong, _T> height;
    ext::html_property<ulong> video_width;
    ext::html_property<ulong> video_height;
    ext::html_property<ext::string, _T> poster;
    ext::html_property<bool, _T> plays_inline;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
    auto qt() const -> QVideoWidget* override;

private accessors:
    auto get_video_width() const -> ulong;
    auto get_video_height() const -> ulong;
};


#endif //SBROWSER_HTML_VIDEO_ELEMENT_HPP
