#pragma once
#ifndef SBROWSER_AUDIO_TRACK_HPP
#define SBROWSER_AUDIO_TRACK_HPP

#include <html/media/abstract_track.hpp>

namespace html::media {class audio_track;}


class html::media::audio_track : public abstract_track
{
public constructors:
    audio_track();

public js_properties:
    ext::html_property<bool> enabled;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_AUDIO_TRACK_HPP
