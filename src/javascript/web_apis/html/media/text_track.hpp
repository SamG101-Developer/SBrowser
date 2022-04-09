#pragma once
#ifndef SBROWSER_TEXT_TRACK_HPP
#define SBROWSER_TEXT_TRACK_HPP

#include <html/media/abstract_track.hpp>

namespace html::helpers {struct media_internals;}
namespace html::media {class text_track;}
namespace html::media {class text_track_cue;}


class html::media::text_track : public abstract_track
{
public friends:
    friend struct helpers::media_internals;

public constructors:
    text_track();

public js_properties:
    ext::html_property<ext::string> in_band_metadata_track_dispatch_type;
    ext::html_property<ext::string> mode;
    ext::html_property<ext::vector<text_track_cue*>*> cues;
    ext::html_property<ext::vector<text_track_cue*>*> active_cues;
    ext::html_property<bool> default_;

public js_methods:
    auto add_cue(text_track_cue* cue) -> void;
    auto remove_cue(text_track_cue* cue) -> void;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_properties:
    std::function<void()> rules_for_updating_text_tracks;

private cpp_properties:
    helpers::media_internals::track_readiness_state m_readiness_state;

private accessors:
    auto set_mode(const ext::string& val) -> void;
};


#endif //SBROWSER_TEXT_TRACK_HPP
