#pragma once
#ifndef SBROWSER_ABSTRACT_TRACK_HPP
#define SBROWSER_ABSTRACT_TRACK_HPP

#include <ext/html_property.hpp>
#include <dom_object.hpp>

namespace html::media {class abstract_track;}


class html::media::abstract_track
        : virtual public dom_object
{
public constructors:
    abstract_track();

public js_properties:
    ext::html_property<ext::string> id;
    ext::html_property<ext::string> kind;
    ext::html_property<ext::string> label;
    ext::html_property<ext::string> language;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_ABSTRACT_TRACK_HPP
