#include "abstract_track.hpp"


html::media::abstract_track::abstract_track() = default;


auto html::media::abstract_track::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<abstract_track>{isolate}
            .inherit<dom_object>()
            .var("id", &abstract_track::id)
            .var("kind", &abstract_track::kind)
            .var("label", &abstract_track::label)
            .var("language", &abstract_track::language)
            .auto_wrap_objects();
}
