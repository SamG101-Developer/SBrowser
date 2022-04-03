#pragma once
#ifndef SBROWSER_HTML_TRACK_ELEMENT_HPP
#define SBROWSER_HTML_TRACK_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_track_element;}
namespace html::tracks {class text_track;}


class html::elements::html_track_element : public html_element
{
public constructors:
    html_track_element();

public static_constants:
    static const unsigned short NONE = 0;
    static const unsigned short LOADING = 1;
    static const unsigned short LOADED = 2;
    static const unsigned short ERROR = 3;

public js_properties:
    ext::html_property<ext::string, _T> kind;
    ext::html_property<ext::string, _T> src;
    ext::html_property<ext::string, _T> srclang;
    ext::html_property<ext::string, _T> label;
    ext::html_property<bool, _T> default_;
    ext::html_property<unsigned short> ready_state;
    ext::html_property<tracks::text_track*> track;

private accessors:
    auto set_kind(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_TRACK_ELEMENT_HPP
