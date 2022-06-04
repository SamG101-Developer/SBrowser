#include "text_track_cue.hpp"


html::media::text_track_cue::text_track_cue()
        : dom::nodes::event_target()
{
    // set the property's values
    id = "";
    pause_on_exit = false;
}


auto html::media::text_track_cue::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<text_track_cue>{isolate}
            .inherit<dom::nodes::event_target>()
            .var("track", &text_track_cue::track)
            .var("id", &text_track_cue::id)
            .var("start_time", &text_track_cue::start_time)
            .var("end_time", &text_track_cue::end_time)
            .var("pause_on_exit", &text_track_cue::pause_on_exit)
            .auto_wrap_objects();
}
