#include "abstract_iterator.hpp"

#include <dom/iterators/node_filter.hpp>
#include <dom/nodes/node.hpp>


dom::iterators::abstract_iterator::abstract_iterator()
        : what_to_show(0)
{
    filter = nullptr;
    root   = nullptr;
}


auto dom::iterators::abstract_iterator::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<abstract_iterator>{isolate}
            .inherit<dom_object>()
            .var("filter", &abstract_iterator::filter, true)
            .var("root", &abstract_iterator::root, true)
            .var("whatToShow", &abstract_iterator::what_to_show, true)
            .auto_wrap_objects();
}
