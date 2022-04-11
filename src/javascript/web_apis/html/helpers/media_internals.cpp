#include "media_internals.hpp"

#include <thread>

#include <ext/infinity.hpp>

#include <dom/nodes/document.hpp>
#include <dom/other/dom_exception.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/event_dispatching.hpp>

#include <html/elements/html_media_element.hpp>
#include <html/media/text_track.hpp>
#include <html/media/text_track_cue.hpp>
#include <html/media/time_ranges.hpp>


auto html::helpers::media_internals::is_eligible_for_autoplay(
        elements::html_media_element* element)
        -> bool
{
    // check a certain set of criteria to determine if the element is eligible for autoplay
    return element->m_can_autoplay_flag
            and element->paused
            and element->autoplay // TODO : ?
            and ~(element->owner_document->m_sandboxing_flag & internal::sandboxing_flags::SANDBOXED_AUTOMATIC_FEAUTURES)
            // TODO and element->owner_document is allowed to use the autoplay feature
}


auto html::helpers::media_internals::has_ended_playback(
        elements::html_media_element* element)
        -> bool
{
    // the element has ended playback if there is no more data, and the playback is at the end for the forwards playing
    // direction (and no looping), or at the start for reverse playback direction
    return element->ready_state > elements::html_media_element::HAVE_NOTHING and (
                    element->m_current_playback_position == element->duration and element->playback_rate > 0 and not element->loop or
                    element->m_current_playback_position == element->m_earliest_possible_position and element->playback_rate < 0);
}


auto html::helpers::media_internals::take_pending_promises(
        elements::html_media_element* element)
        -> ext::vector<webidl::types::promise<void>>
{
    // clear the promises in the media element and return them
    ext::vector<webidl::types::promise<void>> promises {};
    std::swap(element->m_pending_promises, promises);
    return promises;
}


auto html::helpers::media_internals::resolve_pending_play_promises(
        elements::html_media_element* element,
        ext::vector<webidl::types::promise<void>>& promises)
        -> void
{
    // resolve all the promises in the element with undefined (nullptr default)
    promises.for_each([](webidl::types::promise<void>& promise) {promise.resolve();});
}


auto html::helpers::media_internals::reject_pending_play_promises(
        elements::html_media_element* element,
        ext::vector<webidl::types::promise<void>>& promises,
        const dom::other::dom_exception& error)
        -> void
{
    // reject all the promises in the element with the error
    promises.for_each([error](webidl::types::promise<void>& promise) {promise.reject(error);});
}


auto html::helpers::media_internals::notify_about_playing(
        elements::html_media_element* element)
        -> void
{
    // take the promises and create an element task to fire a playing event and resolve the promises
    auto promises = take_pending_promises(element);
    dom::helpers::mutation_observers::queue_media_element_task(element, [&element, &promises] {
        dom::helpers::event_dispatching::fire_event<>("playing", element);
        resolve_pending_play_promises(promises);
    });
}


auto html::helpers::media_internals::internal_play_steps(
        elements::html_media_element* element)
        -> void
{
    // select a resource if the network state is empty
    if (element->network_state == elements::html_media_element::NETWORK_EMPTY)
        resource_selection_algorithm(element);

    // if the resource has ended playback, and it was in a forward direction, seek the media to the earliest position
    if (has_ended_playback(element) and element->playback_rate > 0)
        seek(element, element->m_earliest_possible_position);

    // if the element is paused, handle the steps to start it playing
    if (element->paused)
    {
        // changes the paused state to false
        element->paused = false;

        // handle the change in showing the poster flag
        if (element->m_show_poster_flag)
        {
            // change the flag to false, and march the time on
            element->m_show_poster_flag = false;
            time_marches_on(element);

            // fire a play event at the media element
            dom::helpers::mutation_observers::queue_media_element_task(element, [&element] {dom::helpers::event_dispatching::fire_event<>("play", element);});

            // if there isn't more data on the way, fire a waiting event, otherwise notify about playing
            if (element->ready_state < elements::html_media_element::HAVE_FUTURE_DATA)
                dom::helpers::mutation_observers::queue_media_element_task(element, [element] {dom::helpers::event_dispatching::fire_event<>("waiting", element);});
            else
                notify_about_playing(element);
        }
    }

    // if the element is already playing, and there is future / enough data (on the way)
    else if (element->ready_state > elements::html_media_element::HAVE_CURRENT_DATA)
    {
        // take the pending promises and resolve them
        auto promises = take_pending_promises(element);
        dom::helpers::mutation_observers::queue_media_element_task(element, [&element, &promises] {resolve_pending_play_promises(element, promises);});
    }

    // set the autoplay flag to false, because the media is playing
    element->m_can_autoplay_flag = false;
}


auto html::helpers::media_internals::internal_pause_steps(
        elements::html_media_element* element)
        -> void
{
    // set the element's autoplay flag to false (can't autoplay when paused)
    element->m_can_autoplay_flag = false;

    // only complete the next steps if the element is paused
    if (not element->paused)
    {
        // pause the element, and take the pending promises
        element->paused = true;
        auto promises = take_pending_promises(element);

        // queue a media task which fires a time update and a pause event at the element, and rejects pending promises
        dom::helpers::mutation_observers::queue_media_element_task(element, [&element, &promises] {
            dom::helpers::event_dispatching::fire_event<>("timeupdate", element);
            dom::helpers::event_dispatching::fire_event<>("pause", element);
            reject_pending_play_promises(element, promises, dom::other::dom_exception{"Element paused", ABORT_ERR});
        });

        // set the official position to current position TODO : why?
        element->m_official_playback_position = element->m_current_playback_position;
    }
}


auto html::helpers::media_internals::seek(
        elements::html_media_element* element,
        double new_playback_position,
        const bool approximate_for_speed) -> void
{
    // set the show_poster_flag to false, as the media is being seeked
    element->m_show_poster_flag = false;

    // return if there is no media data
    if (element->ready_state == elements::html_media_element::HAVE_NOTHING)
        return;

    // if the media element is already seeking, abort the previous seeking, and continue
    if (element->seeking)
        ; // TODO : abort other instance of this function running

    // set the seeking to true for the media element
    element->seeking = true;

    // run the rest of the algorithm in a thread
    std::jthread seeking_thread([&element, &new_playback_position] {

        // constrain the new_playback_position to within the earliest position and the media duration
        new_playback_position = std::min(new_playback_position, (double)element->duration);
        new_playback_position = std::max(new_playback_position, element->m_earliest_possible_position);

        // TODO : element->seekable checks (with new_playback_position)

        // fire a seeking event at the media element, and set the current playback position to the new_playback_position
        dom::helpers::mutation_observers::queue_media_element_task(element, [&element] {dom::helpers::event_dispatching::fire_event<>("seeking", element);});
        element->m_current_playback_position = new_playback_position;

        // the seeking has finished, so set the attribute to false, and march the time on
        element->seeking = false;
        time_marches_on(element);

        // fire a timeupdate and seeked event at the media element, as the seeking has finished
        dom::helpers::mutation_observers::queue_media_element_task(element, [&element] {dom::helpers::event_dispatching::fire_event<>("timeupdate", element);});
        dom::helpers::mutation_observers::queue_media_element_task(element, [&element] {dom::helpers::event_dispatching::fire_event<>("seeked", element);});
    });
}


auto html::helpers::media_internals::populate_list_of_pending_tracks(
        elements::html_media_element* element)
        -> void
{
    // add all the non-disabled tracks that are loading to the pending text tracks of the media element
    element->text_tracks
            ->filter([](media::text_track* track) {return track->m_readiness_state == track_readiness_state::LOADING and track->mode != "disabled";})
            .for_each([&element](media::text_track* track) {element->m_pending_text_tracks.append(track);});
}


auto html::helpers::media_internals::are_text_tracks_ready(
        elements::html_media_element* element)
        -> bool
{
    // the text tracks on a meda element are ready when there aren't any pending tracks, end the element isn't blocked
    return element->m_pending_text_tracks.empty() and not element->m_blocked_on_parser_flag;
}


auto html::helpers::media_internals::effective_media_volume(
        elements::html_media_element* element)
        -> double
{
    // TODO : user defined override

    if (element->muted)
        return 0;

    // TODO : ?
}


auto html::helpers::media_internals::is_unbounded_text_track(
        media::text_track_cue* track_cue)
        -> bool
{
    // a text track cue is unbounded if its end time is infinity
    return track_cue->end_time == ext::infinity<double>{};
}


auto html::helpers::media_internals::honour_user_preferences_for_automatic_track_selection(
        elements::html_media_element* element)
        -> void
{
    // select automatic tracks for subtitles / captions, followed by descriptions
    perform_automatic_track_selection(ext::string_vector{"subtitles", "captions"});
    perform_automatic_track_selection(ext::string_vector{"descriptions"});

    // set all the tracks whose kind is chapters / metadata and are defaulted and in disabled mode to hidden mode
    element->text_tracks
            ->filter([](media::text_track* track) {return (track->kind == "chapters" or track->kind == "metadata") and track->default_ and track->mode == "disabled"})
            .for_each([](media::text_track* track) {track->mode = "hidden";});

    // set the flag which shows that automatic selection has happened
    element->m_did_perform_automatic_track_selection_flag = true;
}


auto html::helpers::media_internals::is_normalized_time_range(
        media::time_ranges* time_ranges)
        -> bool
{
    // iterate over the time ranges, starting at the 1st (not 0th) item
    for (auto index = 1; index < time_ranges->length; ++index)
    {
        // check that each consecutive time range has no overlap or touch, and that the length of the time range >= 0
        // (ie return false if the opposite occurs)
        if (time_ranges->start(index) <= time_ranges->end(index - 1) or time_ranges->start(index) > time_ranges->end(index))
            return false;
    }

    // otherwise, return true, as the time ranges are normalized
    return true;
}
