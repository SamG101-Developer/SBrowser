#include "static_range.hpp"

#include <dom/helpers/exceptions.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document_type.hpp>


dom::ranges::static_range::static_range(ext::cstring_any_map& init) {

    helpers::exceptions::throw_v8_exception(
            "range containers must be non-attr nodes",
            INVALID_NODE_TYPE_ERR,
            [init] {return ext::any_cast<nodes::attr*>(init.at("startContainer")) or ext::any_cast<nodes::attr*>(init.at("endContainer"));});

    helpers::exceptions::throw_v8_exception(
            "range containers must be non-document_type nodes",
            INVALID_NODE_TYPE_ERR,
            [init] {return ext::any_cast<nodes::document_type*>(init.at("startContainer")) or ext::any_cast<nodes::document_type*>(init.at("endContainer"));});

    start_container = ext::any_cast<nodes::node*>(init.at("startContainer"));
    start_offset = ext::any_cast<unsigned long>(init.at("startOffset"));
    end_container = ext::any_cast<nodes::node*>(init.at("endContainer"));
    end_offset = ext::any_cast<unsigned long>(init.at("endOffset"));
}
