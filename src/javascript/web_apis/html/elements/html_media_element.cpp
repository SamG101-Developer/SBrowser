#include "html_media_element.hpp"

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/other/dom_exception.hpp>

#include <html/events/track_event.hpp>
#include <html/helpers/custom_html_elements.hpp>
#include <html/helpers/media_internals.hpp>
#include <html/media/text_track.hpp>

#include <web_idl/types/date.hpp>
#include <web_idl/types/promise.hpp>


html::elements::html_media_element::html_media_element()
        : html_element()
{
    // set the custom accessors
    current_time.getter = [this] {return get_current_time();};

    current_time.setter = [this](auto&& PH1) {set_current_time(std::forward<decltype(PH1)>(PH1));};
    ready_state.setter = [this](auto&& PH1) {set_ready_state(std::forward<decltype(PH1)>(PH1));};

    // set the properties
    text_tracks << new ext::vector<html::media::text_track*>{};

    // set the attributes
    m_current_playback_position = 0.0;
    m_official_playback_position = 0.0;
    m_default_playback_start_position = 0.0;
    m_earliest_possible_position = 0.0;
    m_playback_volume = 1.0; // TODO : remember across sessions (config file / QSettings?)
}


auto html::elements::html_media_element::load()
        -> void
{
    // load the media using the helper method
    helpers::media_internals::media_element_load_algorithm(this);
}


auto html::elements::html_media_element::pause()
        -> void
{
    // if network state is empty, then select the correct media source using the helper method
    if (network_state == NETWORK_EMPTY)
        helpers::media_internals::resource_selection_algorithm(this);

    // pause the media using the helper method
    html::helpers::media_internals::internal_pause_steps(this);
}


auto html::elements::html_media_element::play()
        -> webidl::types::promise<void>
{
    // create a promise that will be resolved or rejected depending on if the media is allowed to play or not
    webidl::types::promise<void> promise{};

    // if the media is not allowed to play
    if (not helpers::media_internals::is_allowed_to_play(this))
    {
        // reject the promise with a not allowed error, and return the promise
        promise.reject(dom::other::dom_exception{"Media not allowed to play", NOT_ALLOWED_ERR});
        return promise;
    }

    // TODO : media error check here

    // append the promise to the pending promises, run the internal play steps, and return the promise
    m_pending_promises.append(promise);
    helpers::media_internals::internal_play_steps(this);
    return promise;
}


auto html::elements::html_media_element::can_play_type(
        const ext::string& type)
        -> ext::string
{
    return ""; // TODO
}


auto html::elements::html_media_element::fast_seek(
        double time)
        -> void
{
    helpers::media_internals::seek(this, time, true);
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
    text_track->m_readiness_state = helpers::media_internals::track_readiness_state::LOADING;

    // append tbe text track to the class's text tracks
    text_tracks->append(text_track);

    // queue a media element task to fire a track addtrack event at the track
    dom::helpers::mutation_observers::queue_media_element_task(this, [this, &text_track](){
        dom::helpers::event_dispatching::fire_event<events::track_event>("addtrack", text_tracks, {{"track", text_track}});
    });

    // return the text tracks
    return *text_tracks;
}


auto html::elements::html_media_element::set_ready_state(
        ushort val)
        -> void
{
    ushort previous_ready_state;
    ready_state >> previous_ready_state;
    ready_state << val;

    if (previous_ready_state != NETWORK_EMPTY)
    {
        // if the ready state was nothing, and is becoming metadata, fire a loaded metadata event
        if (previous_ready_state == HAVE_NOTHING and ready_state == HAVE_METADATA)
        {
            dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("loadedmetadata", this);});
            return;
        }

        // if the ready state was metadata and now is anything more, fire a loaded metadata event
        if (previous_ready_state == HAVE_METADATA and ready_state > HAVE_METADATA)
        {
            if (/* TODO */ true)
                dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("loadeddata", this);});
            if (ready_state != HAVE_FUTURE_DATA or ready_state != HAVE_ENOUGH_DATA) return;
        }

        // if the ready state was current data and is now anything less, fire a [timeupdate, waiting] event
        if (previous_ready_state > HAVE_CURRENT_DATA and ready_state <= HAVE_CURRENT_DATA)
        {
            if (helpers::media_internals::is_potentially_playing(this) and not helpers::media_internals::has_ended_playback(this))
            {
                dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("timeupdate", this);});
                dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("waiting", this);});
            }
        }

        // if the ready state was current data and is now future data (ie more incoming), fire a canplay event
        if (previous_ready_state == HAVE_CURRENT_DATA and ready_state == HAVE_FUTURE_DATA)
        {
            dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("canplay", this);});
            if (not paused) helpers::media_internals::notify_about_playing(this);
        }

        // if the ready state is now enough data
        if (ready_state == HAVE_ENOUGH_DATA)
        {
            // case for if the ready state was current data
            if (previous_ready_state == HAVE_CURRENT_DATA)
            {
                // fire a canplay event, and notify about playing if the paused attribute isn't set
                dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("canplay", this);});
                if (not paused) helpers::media_internals::notify_about_playing(this);

                // fire a canplaythrough event, and return if the element isn't eligible for autoplay
                dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("canplaythrough", this);});
                if (not helpers::media_internals::is_eligible_for_autoplay(this)) return;

                // unset the paused attribute, and march time on if the show poster flag is set (move into media?)
                paused = false;
                if (m_show_poster_flag)
                {
                    m_show_poster_flag = false;
                    helpers::media_internals::time_marches_on(this);
                }

                // fire a play event, and notify about playing
                dom::helpers::mutation_observers::queue_media_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("play", this);});
                helpers::media_internals::notify_about_playing(this);
            }
        }

    }
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
            .function("canPlayTime", &html_media_element::can_play_type)
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
