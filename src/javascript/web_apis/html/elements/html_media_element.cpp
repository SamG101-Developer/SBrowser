#include "html_media_element.hpp"

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <html/events/track_event.hpp>
#include <html/helpers/custom_html_elements.hpp>
#include <web_idl/types/date.hpp>
#include <web_idl/types/promise.hpp>


html::elements::html_media_element::html_media_element()
        : html_element()
{
    // set the custom accessors
    current_time.getter = [this] {return get_current_time();};
    current_time.setter = [this](auto&& PH1) {set_current_time(std::forward<decltype(PH1)>(PH1));};

    // set the properties
    text_tracks << new ext::vector<html::media::text_track*>{};

    // set the attributes
    m_current_playback_position = 0.0;
    m_official_playback_position = 0.0;
    m_default_playback_start_position = 0.0;
    m_earliest_possible_position = 0.0;


}


auto html::elements::html_media_element::load()
        -> void
{
    // load the media using the helper method
    helpers::media::media_element_load_algorithm(this);
}


auto html::elements::html_media_element::pause()
        -> void
{
    // if network state is empty, then select the correct media source using the helper method
    if (network_state == NETWORK_EMPTY)
        helpers::media::resource_selection_algorithm(this);

    // pause the media using the helper method
    html::helpers::media::internal_pause_steps(this);
}


auto html::elements::html_media_element::play()
        -> webidl::types::promise<void>
{}


auto html::elements::html_media_element::can_play_time(
        const ext::string& type)
        -> ext::string
{
    return ""; // TODO
}


auto html::elements::html_media_element::fast_seek(
        double time)
        -> void
{
    helpers::media::seek(this, time, true);
}


auto html::elements::html_media_element::get_start_date()
        -> webidl::types::date
{
    return webidl::types::date{m_timeline_offset};
}


auto html::elements::html_media_element::add_text_track(
        const ext::string& kind,
        const ext::string& label,
        const ext::string& language)
        -> ext::vector<html::media::text_track*>
{
    // create the text track
    auto* text_track = new media::text_track{};
    text_track->kind = kind;
    text_track->label = label;
    text_track->language = language;
    text_track->mode = "hidden";
    text_track->cues = new ext::vector<media::text_track_cue*>{};
    text_track->m_readiness_state = "loaded";

    // append tbe text track to the class's text tracks
    text_tracks->append(text_track);

    // queue the media element on the observers and return the text tracks
    dom::helpers::mutation_observers::queue_media_element_task(this, [this](){dom::helpers::event_dispatching::fire_event<events::track_event>("addTrack", text_tracks->front());});
    return *text_tracks;
}


auto html::elements::html_media_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_media_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .function("load", &html_media_element::load)
            .function("pause", &html_media_element::pause)
//            .function("play", &html_media_element::play)
            .function("canPlayTime", &html_media_element::can_play_time)
            .function("fastSeek", &html_media_element::fast_seek)
//            .function("getStartDate", &html_media_element::get_start_date)
            .function("addTextTrack", &html_media_element::add_text_track)
            .var("error", &html_media_element::error)
            .var("srcObject", &html_media_element::src_object)
            .var("buffered", &html_media_element::buffered)
            .var("src", &html_media_element::src)
            .var("currentSrc", &html_media_element::current_src)
            .var("crossOrigin", &html_media_element::cross_origin)
            .var("preload", &html_media_element::preload)
            .var("networkState", &html_media_element::network_state)
            .var("readyState", &html_media_element::ready_state)
            .var("seeking", &html_media_element::seeking)
            .var("paused", &html_media_element::paused)
            .var("preservesPitch", &html_media_element::preserves_pitch)
            .var("ended", &html_media_element::ended)
            .var("autoplay", &html_media_element::autoplay)
            .var("loop", &html_media_element::loop)
            .var("currentTime", &html_media_element::current_time)
            .var("duration", &html_media_element::duration)
            .var("defaultPlaybackRate", &html_media_element::default_playback_rate)
            .var("playbackRate", &html_media_element::playback_rate)
            .var("played", &html_media_element::played)
            .var("seekable", &html_media_element::seekable)
            .var("controls", &html_media_element::controls)
            .var("defaultMuted", &html_media_element::default_muted)
            .var("muted", &html_media_element::muted)
            .var("volume", &html_media_element::volume)
            .var("audioTracks", &html_media_element::audio_tracks)
            .var("videoTracks", &html_media_element::video_tracks)
            .var("textTracks", &html_media_element::text_tracks)
            .auto_wrap_objects();
}
