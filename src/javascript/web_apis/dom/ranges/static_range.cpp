#include "static_range.hpp"

#include <dom/helpers/exceptions.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document_type.hpp>


dom::ranges::static_range::static_range() = default;


dom::ranges::static_range::static_range(
        const ext::string_any_map_t& init)
{
    // if the start container or end container are attribute nodes, then throw an invalid node type error
    helpers::exceptions::throw_v8_exception<INVALID_NODE_TYPE_ERR>(
            "range containers must be non-attr nodes",
            [init] {return init.at("startContainer").to<nodes::attr*>() or init.at("endContainer").to<nodes::attr*>();});

    // if the start container or end container are document type nodes, then throw an invalid node type error
    helpers::exceptions::throw_v8_exception<INVALID_NODE_TYPE_ERR>(
            "range containers must be non-document_type nodes",
            [init] {return init.at("startContainer").to<nodes::document_type*>() or init.at("endContainer").to<nodes::document_type*>();});

    // set the start and end containers and offsets from the options map
    start_container = init.at("startContainer").to<nodes::node*>();
    start_offset = init.at("startOffset").to<ulong>();
    end_container = init.at("endContainer").to<nodes::node*>();
    end_offset = init.at("endOffset").to<ulong>();
}


auto dom::ranges::static_range::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<static_range>{isolate}
            .ctor<const ext::string_any_map_t&>()
            .inherit<abstract_range>()
            .auto_wrap_objects();
}
