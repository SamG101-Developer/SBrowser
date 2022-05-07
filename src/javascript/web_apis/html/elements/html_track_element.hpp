#pragma once
#ifndef SBROWSER_HTML_TRACK_ELEMENT_HPP
#define SBROWSER_HTML_TRACK_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_track_element;}
namespace html::media {class text_track;}


class html::elements::html_track_element : public html_element
{
public constructors:
    html_track_element();

public js_static_constants:
    static const ushort NONE = 0;
    static const ushort LOADING = 1;
    static const ushort LOADED = 2;
    static const ushort ERROR = 3;

public js_properties:
    ext::html_property<ext::string, _T> kind;
    ext::html_property<ext::string, _T> src;
    ext::html_property<ext::string, _T> srclang;
    ext::html_property<ext::string, _T> label;
    ext::html_property<bool, _T> default_;
    ext::html_property<ushort, _F> ready_state;
    ext::html_property<media::text_track*, _F> track;

private accessors:
    auto set_parent_node(dom::nodes::node* val) -> void override;
};


#endif //SBROWSER_HTML_TRACK_ELEMENT_HPP
