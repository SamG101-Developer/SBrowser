#include "abstract_range.hpp"


dom::ranges::abstract_range::abstract_range() : dom_object()
{
}


auto dom::ranges::abstract_range::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<abstract_range>{isolate}
            .template inherit<dom_object>()
            .template var("collapsed", &abstract_range::collapsed)
            .template var("startContainer", &abstract_range::start_container)
            .template var("startOffset", &abstract_range::start_offset)
            .template var("endContainer", &abstract_range::end_container)
            .template var("endOffset", &abstract_range::end_offset)
            .auto_wrap_objects();
}



