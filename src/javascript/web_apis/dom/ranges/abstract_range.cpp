#include "abstract_range.hpp"


dom::ranges::abstract_range::abstract_range() : dom_object()
{
}


ext::any
dom::ranges::abstract_range::v8(v8::Isolate* isolate) const
{
    return v8pp::class_<abstract_range>{isolate}
            .inherit<dom_object>()

            .var("collapsed", &abstract_range::collapsed)
            .var("startContainer", &abstract_range::start_container)
            .var("startOffset", &abstract_range::start_offset)
            .var("endContainer", &abstract_range::end_container)
            .var("endOffset", &abstract_range::end_offset)

            .auto_wrap_objects();
}



