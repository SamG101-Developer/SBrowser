#include "text_track.hpp"

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/trees.hpp>

#include <html/elements/html_media_element.hpp>
#include <html/helpers/media_internals.hpp>
#include <html/media/text_track_cue.hpp>


html::media::text_track::text_track()
        : abstract_track()
{
    // constrain the attribute values
    kind.constrain_values({"subtitles", "descriptions", "chapters", "metadata"});
    mode.constrain_values({"disabled", "hidden", "showing"});

    // set the custom accessors
    mode.setter = [this](auto&& PH1) {set_mode(std::forward<decltype(PH1)>(PH1));};

    // set the attributes
    m_readiness_state = helpers::media_internals::track_readiness_state::NOT_LOADED;
}


auto html::media::text_track::add_cue(
        text_track_cue* cue)
        -> void
{
    // TODO : associated rules x2
    // TODO : ?

    // append the cue into the list of cues
    cues->append(cue);
}


auto html::media::text_track::remove_cue(
        text_track_cue* cue)
        -> void
{
    // if the cue isn't in the list, throw a not found error
    dom::helpers::exceptions::throw_v8_exception<NOT_FOUND_ERR>(
            "Cue not in list",
            [this, &cue] {return not cues->contains(cue);});

    // remove the cue from the list of cues (auto c++ check for if the item is in the list or not)
    cues->remove(cue);
}


auto html::media::text_track::set_mode(
        const ext::string& val)
        -> void
{
    // get the media element containing this text track TODO
    elements::html_media_element* element = nullptr;

    // return if the pending text track change notification flag is set, otherwise set it
    if (element->m_pending_text_track_change_notification_flag)
        return;
    element->m_pending_text_track_change_notification_flag = true;

    // queue a media element task to unset the flag, and fire a change event at the text track list
    dom::helpers::mutation_observers::queue_media_element_task(element, [&element] {
        element->m_pending_text_track_change_notification_flag = false;
        dom::helpers::event_dispatching::fire_event<>("change", element->text_tracks);
    });

    // if the show poster flag is set, march the time on
    if (element->m_show_poster_flag)
        helpers::media_internals::time_marches_on(element);

    mode << val;
}


auto html::media::text_track::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<text_track>{isolate}
            .inherit<abstract_track>()
            .function("addCue", &text_track::add_cue)
            .function("removeCue", &text_track::remove_cue)
            .var("inBandMetadataTrackDispatchType", &text_track::in_band_metadata_track_dispatch_type)
            .var("mode", &text_track::mode)
            .var("cues", &text_track::cues)
            .var("active_cues", &text_track::active_cues)
            .var("default", &text_track::default_)
            .auto_wrap_objects();
}
