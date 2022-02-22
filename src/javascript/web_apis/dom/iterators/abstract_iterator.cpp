#include "abstract_iterator.hpp"


dom::iterators::abstract_iterator::abstract_iterator() = default;


auto dom::iterators::abstract_iterator::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<abstract_iterator>{isolate}
            .inherit<dom_object>()
            .var("root", &dom::iterators::abstract_iterator::root)
            .var("filter", &dom::iterators::abstract_iterator::filter)
            .var("whatToShow", &dom::iterators::abstract_iterator::what_to_show)
            .auto_wrap_objects();
}
