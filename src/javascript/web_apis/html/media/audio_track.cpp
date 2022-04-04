#include "audio_track.hpp"


html::media::audio_track::audio_track() = default;


auto html::media::audio_track::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<audio_track>{isolate}
            .inherit<abstract_track>()
            .var("enabled", &audio_track::enabled)
            .auto_wrap_objects();
}
