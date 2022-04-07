#pragma once
#ifndef SBROWSER_HTML_MEDIA_ELEMENT_HPP
#define SBROWSER_HTML_MEDIA_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <web_idl/types/promise.hpp>
#include <web_idl/types/date.hpp>

namespace html::elements {class html_media_element;}
namespace html::elements {class html_track_element;}
namespace html::helpers {struct media_internals;}
namespace html::media{class audio_track;}
namespace html::media{class media_error;}
namespace html::media{class media_provider;}
namespace html::media{class text_track;}
namespace html::media{class text_track_cue;}
namespace html::media{class time_ranges;}
namespace html::media{class video_track;}


class html::elements::html_media_element : public html_element
{
public friends:
    friend struct helpers::media_internals;
    friend class html_track_element;
    friend class media::text_track;

public constructors:
    html_media_element();

    static const unsigned short NETWORK_EMPTY     = 0;
    static const unsigned short NETWORK_IDLE      = 1;
    static const unsigned short NETWORK_LOADING   = 2;
    static const unsigned short NETWORK_NO_SOURCE = 3;

    static const unsigned short HAVE_NOTHING      = 0;
    static const unsigned short HAVE_METADATA     = 1;
    static const unsigned short HAVE_CURRENT_DATA = 2;
    static const unsigned short HAVE_FUTURE_DATA  = 3;
    static const unsigned short HAVE_ENOUGH_DATA  = 4;

public js_methods:
    auto load() -> void;
    auto pause() -> void;
    auto play() -> webidl::types::promise<void>;
    auto can_play_type(const ext::string& type) -> ext::string;
    auto fast_seek(double time) -> void;
    auto get_start_date() -> webidl::types::date;
    auto add_text_track(const ext::string& kind, const ext::string& label = "", const ext::string& language = "") -> ext::vector<html::media::text_track*>;

public js_properties:
    ext::html_property<html::media::media_error*> error;

    ext::html_property<ext::string, _T> src;
    ext::html_property<html::media::media_provider*> src_object;
    ext::html_property<ext::string> current_src;
    ext::html_property<ext::string, _T> cross_origin;
    ext::html_property<unsigned short> network_state;
    ext::html_property<ext::string, _T> preload;
    ext::html_property<html::media::time_ranges*> buffered;

    ext::html_property<unsigned short> ready_state;
    ext::html_property<bool> seeking;

    ext::html_property<double> current_time;
    ext::html_property<double> duration;
    ext::html_property<double> default_playback_rate;
    ext::html_property<double> playback_rate;
    ext::html_property<bool, _T> autoplay;
    ext::html_property<bool, _T> loop;
    ext::html_property<bool> paused;
    ext::html_property<bool> preserves_pitch;
    ext::html_property<bool> ended;
    ext::html_property<html::media::time_ranges*> played;
    ext::html_property<html::media::time_ranges*> seekable;

    // Controls
    ext::html_property<bool, _T> controls;
    ext::html_property<bool, _T> default_muted;
    ext::html_property<bool> muted;
    ext::html_property<double> volume;

    // Tracks
    ext::html_property<ext::vector<html::media::audio_track*>*> audio_tracks;
    ext::html_property<ext::vector<html::media::video_track*>*> video_tracks;
    ext::html_property<ext::vector<html::media::text_track*>*> text_tracks;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected cpp_properties:
    html::elements::html_media_element* m_media_provider_resource;
    html::media::media_provider* m_assigned_media_provider_object;
    // javaScript::scripting::media_task_source* m_elementEventTaskSource; // TOTO -> check type

    double m_media_timeline;
    double m_timeline_offset;
    double m_current_playback_position;
    double m_official_playback_position;
    double m_default_playback_start_position;
    double m_earliest_possible_position;
    double m_playback_volume;
    double m_effective_media_volume;

    bool m_show_poster_flag = false;
    bool m_can_autoplay_flag = false;
    bool m_delaying_the_load_event_flag = false;
    bool m_blocked_on_parser_flag = false;
    bool m_did_perform_automatic_track_selection_flag = false;
    bool m_pending_text_track_change_notification_flag = false;

    ext::vector<webidl::types::promise<void>> m_pending_promises;
    ext::vector<html::media::text_track*> m_pending_text_tracks;
    ext::vector<html::media::text_track_cue*> m_newly_introduced_cues;

private accessors:
    auto get_current_time() const -> double;
    auto get_effective_media_volume() const -> double;

    auto set_ready_state(unsigned short val) -> void;
    auto set_current_time(double val) -> void;
};


#endif //SBROWSER_HTML_MEDIA_ELEMENT_HPP
