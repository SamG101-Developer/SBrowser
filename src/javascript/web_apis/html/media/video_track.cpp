#include "video_track.hpp"


html::media::video_track::video_track() = default;


auto html::media::video_track::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<video_track>{isolate}
            .inherit<abstract_track>()
            .var("selected", &video_track::selected)
            .auto_wrap_objects();
}
