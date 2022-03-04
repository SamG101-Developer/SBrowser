#pragma once
#ifndef SBROWSER_HTML_MEDIA_ELEMENT_HPP
#define SBROWSER_HTML_MEDIA_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html
{
    namespace elements {class html_media_element;}
    namespace media
    {
        class audio_track;
        class media_error;
        class media_provider;
        class text_track;
        class text_track_cue;
        class time_ranges;
        class video_track;
    }
}

namespace webidl::types
{
    template <typename T> class promise;
    class date;
}


class html::elements::html_media_element : public html_element
{
public: constructors
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

public: methods
    auto load() -> void;
    auto pause() -> void;
    auto play() -> webidl::types::promise<void>;
    auto can_play_time(const ext::string& type) -> ext::string;
    auto fast_seek(double time) -> void;
    auto get_start_date() -> webidl::types::date;
    auto add_text_track(const ext::string& kind, const ext::string& label = "", const ext::string& language = "") -> ext::vector<html::media::text_track*>;

public: properties
    // Errors
    ext::dom_property<html::media::media_error*, _F> error;

    // Networking
    ext::dom_property<html::media::media_provider*, _F> src_object;
    ext::dom_property<html::media::time_ranges*, _F> buffered;
    ext::dom_property<ext::string, _T> src;
    ext::dom_property<ext::string, _F> current_src;
    ext::dom_property<ext::string, _T> cross_origin;
    ext::dom_property<ext::string, _T> preload;
    ext::dom_property<unsigned short, _F> network_state;

    // Readiness
    ext::dom_property<unsigned short, _F> ready_state;
    ext::dom_property<bool, _F> seeking;

    // Playback
    ext::dom_property<bool, _F> paused;
    ext::dom_property<bool, _F> preserves_pitch;
    ext::dom_property<bool, _F> ended;
    ext::dom_property<bool, _T> autoplay;
    ext::dom_property<bool, _T> loop;
    ext::dom_property<double, _F> current_time;
    ext::dom_property<double, _F> duration;
    ext::dom_property<double, _F> default_playback_rate;
    ext::dom_property<double, _F> playback_rate;
    ext::dom_property<html::media::time_ranges*, _F> played;
    ext::dom_property<html::media::time_ranges*, _F> seekable;

    // Controls
    ext::dom_property<bool, _T> controls;
    ext::dom_property<bool, _T> default_muted;
    ext::dom_property<bool, _F> muted;
    ext::dom_property<double, _F> volume;

    // Tracks
    ext::dom_property<ext::vector<html::media::audio_track*>*, _F> audio_tracks;
    ext::dom_property<ext::vector<html::media::video_track*>*, _F> video_tracks;
    ext::dom_property<ext::vector<html::media::text_track*>*, _F> text_tracks;

protected: internal_properties
    html::elements::html_media_element* m_media_provider_resource;
    html::media::media_provider* m_assigned_media_provider_object;
    // javaScript::scripting::media_task_source* m_elementEventTaskSource; // TOTO -> check type

    double m_media_timeline;
    double m_timeline_offset;
    double m_current_playback_position;
    double m_official_playback_position;
    double m_default_playback_start_position;
    double m_earliest_possible_position;
    double m_effective_media_volume;

    ext::vector<webidl::types::promise<void>*> m_pending_promises;
    ext::vector<html::media::text_track*> m_pending_text_tracks;
    ext::vector<html::media::text_track_cue*> m_newly_introduced_cues;

private: accessors
    auto get_current_time() const -> double;
    auto get_effective_media_volume() const -> double;

    auto set_current_time(double val) -> void;
};


#endif //SBROWSER_HTML_MEDIA_ELEMENT_HPP
