#include "html_media_element.hpp"

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <html/helpers/custom_html_elements.hpp>
#include <web_idl/types/date.hpp>
#include <web_idl/types/promise.hpp>


html::elements::html_media_element::html_media_element()
        : html_element()
{
    HTML_CONSTRUCTOR

    // set the custom accessors
    current_time.getter = [this] {return get_current_time();};
    current_time.setter = [this](auto&& PH1) {set_current_time(std::forward<decltype(PH1)>(PH1));};

    text_tracks = new ext::vector<html::media::text_track*>{};

    m_current_playback_position = 0.0;
    m_official_playback_position = 0.0;
    m_default_playback_start_position = 0.0;
    m_earliest_possible_position = 0.0;
}


auto html::elements::html_media_element::load() -> void
{
    helpers::media::media_element_load_algorithm(this);
}


auto html::elements::html_media_element::pause() -> void
{
    if (network_state == NETWORK_EMPTY)
        helpers::media::resource_selection_algorithm(this);
    html::helpers::media::internal_pause_steps(this);
}


auto html::elements::html_media_element::play() -> webidl::types::promise<void>
{}


auto html::elements::html_media_element::can_play_time(const ext::string& type) -> ext::string
{
    return ""; // TODO
}


auto html::elements::html_media_element::fast_seek(double time) -> void
{
    helpers::media::seek(this, time, true);
}


auto html::elements::html_media_element::get_start_date() -> webidl::types::date
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
    dom::helpers::mutation_observers::queue_media_element_task(this, [this]() {dom::helpers::event_dispatching::fire_event<events::track_event>("addTrack", text_tracks->front());});
    return *text_tracks;
}
