#include "abstract_iterator.hpp"


dom::iterators::abstract_iterator::abstract_iterator() = default;


auto dom::iterators::abstract_iterator::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<abstract_iterator>{isolate}
            .template inherit<dom_object>()
            .template var("root", &abstract_iterator::root)
            .template var("filter", &abstract_iterator::filter)
            .template var("whatToShow", &abstract_iterator::what_to_show)
            .auto_wrap_objects();
}
