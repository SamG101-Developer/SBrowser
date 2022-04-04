#pragma once
#ifndef SBROWSER_TEXT_TRACK_HPP
#define SBROWSER_TEXT_TRACK_HPP

#include <html/media/abstract_track.hpp>
#include <html/helpers/media_internals.hpp>

namespace html::media {class text_track;}
namespace html::media {class text_track_cue;}


class html::media::text_track : public abstract_track
{
public constructors:
    text_track();

public js_properties:
    ext::html_property<ext::string> in_band_metadata_track_dispatch_type;
    ext::html_property<ext::string> mode;
    ext::html_property<ext::vector<text_track_cue*>*> cues;
    ext::html_property<ext::vector<text_track_cue*>*> active_cues;

public js_methods:
    auto add_cue(text_track_cue* cue) -> void;
    auto remove_cue(text_track_cue* cue) -> void;

protected cpp_properties:
    std::function<void()> rules_for_updating_text_tracks;

private cpp_properties:
    helpers::media_internals::track_readiness_state m_readiness_state;
    ext::vector<text_track_cue*> m_pending_text_tracks = {};
    bool m_blocked_on_parser_flag = false;
    bool m_did_perform_automatic_track_selection_flag = false;
};


#endif //SBROWSER_TEXT_TRACK_HPP
