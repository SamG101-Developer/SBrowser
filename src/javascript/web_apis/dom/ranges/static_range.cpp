#include "static_range.hpp"

#include <dom/helpers/exceptions.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document_type.hpp>


dom::ranges::static_range::static_range(ext::cstring_any_map& init) {

    helpers::exceptions::throw_v8_exception(
            "range containers must be non-attr nodes",
            INVALID_NODE_TYPE_ERR,
            [init] {return init.at("startContainer").to<nodes::attr*>() or init.at("endContainer").to<nodes::attr*>();});

    helpers::exceptions::throw_v8_exception(
            "range containers must be non-document_type nodes",
            INVALID_NODE_TYPE_ERR,
            [init] {return init.at("startContainer").to<nodes::document_type*>() or init.at("endContainer").to<nodes::document_type*>();});

    start_container = init.at("startContainer").to<nodes::node*>();
    start_offset = init.at("startOffset").to<unsigned long>();
    end_container = init.at("endContainer").to<nodes::node*>();
    end_offset = init.at("endOffset").to<unsigned long>();
}


auto dom::ranges::static_range::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<static_range>{isolate}
            .ctor<ext::cstring_any_map&>()
            .inherit<abstract_range>()
            .auto_wrap_objects();
}
