#pragma once
#ifndef SBROWSER_MEDIA_INTERNALS_HPP
#define SBROWSER_MEDIA_INTERNALS_HPP

#include <ext/vector.hpp>
#include <web_idl/types/promise.hpp>

namespace dom::other {class dom_exception;}
namespace html::elements {class html_media_element;}
namespace html::helpers {struct media_internals;}
namespace html::media {class text_track_cue;}
namespace html::media {class time_ranges;}


struct html::helpers::media_internals
{
    static auto media_element_load_algorithm(
            elements::html_media_element* element)
            -> void;

    static auto resource_selection_algorithm(
            elements::html_media_element* element)
            -> void;

    static auto dedicated_media_resource_failure_steps(
            elements::html_media_element* element,
            ext::vector<webidl::types::promise<void>>& promises)
            -> void;

    static auto resource_fetch_algorithm(
            elements::html_media_element* element,
            const ext::string& url)
            -> void;

    static auto is_blocked_media_element(
            elements::html_media_element* element)
            -> bool;

    static auto is_potentially_playing(
            elements::html_media_element* element)
            -> bool;

    static auto is_eligible_for_autoplay(
            elements::html_media_element* element)
            -> bool;

    static auto is_allowed_to_play(
            elements::html_media_element* element)
            -> bool;

    static auto has_ended_playback(
            elements::html_media_element* element)
            -> bool;

    static auto has_stopped_for_errors(
            elements::html_media_element* element)
            -> bool;

    static auto paused_for_user_interation(
            elements::html_media_element* element)
            -> bool;

    static auto paused_for_inband_content(
            elements::html_media_element* element)
            -> bool;

    static auto take_pending_promises(
            elements::html_media_element* element)
            -> ext::vector<webidl::types::promise<void>>;

    static auto resolve_pending_play_promises(
            elements::html_media_element* element,
            ext::vector<webidl::types::promise<void>>& promises)
            -> void;

    static auto reject_pending_play_promises(
            elements::html_media_element* element,
            ext::vector<webidl::types::promise<void>>& promises,
            const dom::other::dom_exception& error)
            -> void;

    static auto notify_about_playing(
            elements::html_media_element* element)
            -> void;

    static auto internal_play_steps(
            elements::html_media_element* element)
            -> void;

    static auto internal_pause_steps(
            elements::html_media_element* element)
            -> void;

    static auto time_marches_on(
            elements::html_media_element* element)
            -> void;

    static auto seek(
            elements::html_media_element* element,
            double new_playback_position,
            bool approximate_for_speed = false)
            -> void;

    static auto populate_list_of_pending_tracks(
            elements::html_media_element* element)
            -> void;

    static auto are_text_tracks_ready(
            elements::html_media_element* element)
            -> bool;

    static auto effective_media_volume(
            elements::html_media_element* element)
            -> double;

    static auto is_unbounded_text_track(
            media::text_track_cue* track_cue)
            -> bool;

    static auto expose_a_media_resource_specific_text_track(
            elements::html_media_element* element,
            const ext::string& relevant_data)
            -> void;

    static auto honour_user_preferences_for_automatic_track_selection(
            elements::html_media_element* element)
            -> void;

    static auto perform_automatic_track_selection(
            const ext::string_vector& text_track_kinds)
            -> void;

    static auto start_track_processing_model(
            media::text_track* track,
            elements::html_track_element* element)
            -> void;

    static auto is_normalized_time_range(
            media::time_ranges* time_ranges)
            -> bool;

    enum class track_readiness_state
    {
        NOT_LOADED, LOADING, LOADED, FAILED_TO_LOAD
    };
};


#endif //SBROWSER_MEDIA_INTERNALS_HPP
