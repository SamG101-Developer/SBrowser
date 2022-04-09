#include "time_ranges.hpp"

#include <dom/helpers/exceptions.hpp>


html::media::time_ranges::time_ranges() = default;


auto html::media::time_ranges::start(
        ulong index) const
        -> double
{
    // if the index is >= the length of the list, throw an index size error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "index must be < length of list",
            [this, index] {return index >= length;});

    // return the start of the range at the index
    return m_linked_list->at(index).first;
}


auto html::media::time_ranges::end(
        ulong index) const
        -> double
{
    // if the index is >= the length of the list, throw an index size error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "index must be < length of list",
            [this, index] {return index >= length;});

    // return the end of the range at the index
    return m_linked_list->at(index).second;
}


auto html::media::time_ranges::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<time_ranges>{isolate}
            .inherit<dom_object>()
            .inherit<ext::listlike<std::pair<int, int>>>()
            .function("start", &time_ranges::start)
            .function("end", &time_ranges::end)
            .auto_wrap_objects();
}
