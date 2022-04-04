#pragma once
#ifndef SBROWSER_VIDEO_TRACK_HPP
#define SBROWSER_VIDEO_TRACK_HPP

#include <html/media/abstract_track.hpp>

namespace html::media {class video_track;}


class html::media::video_track : public abstract_track
{
public constructors:
    video_track();

public js_properties:
    ext::html_property<bool> selected;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_VIDEO_TRACK_HPP
