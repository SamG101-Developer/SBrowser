#pragma once
#ifndef SBROWSER_NODE_FILTER_HPP
#define SBROWSER_NODE_FILTER_HPP

#include <dom_object.hpp>

namespace dom::iterators {struct node_filter;}

namespace dom::nodes {class node;}


struct dom::iterators::node_filter final : public virtual dom_object
{
public aliases:
    using accept_callback_t = std::function<ushort(const nodes::node*)>;

public constructors:
    node_filter();

public js_static_constants:
    static const ushort FILTER_ACCEPT = 1;
    static const ushort FILTER_REJECT = 2;
    static const ushort FILTER_SKIP   = 3;

    static const ulong SHOW_ELEMENT                 = 0x1;
    static const ulong SHOW_ATTRIBUTE               = 0x2;
    static const ulong SHOW_TEXT                    = 0x4;
    static const ulong SHOW_CDATA_SECTION           = 0x8;
    static const ulong SHOW_PROCESSING_INSTRUCTION  = 0x40;
    static const ulong SHOW_COMMENT                 = 0x80;
    static const ulong SHOW_DOCUMENT                = 0x100;
    static const ulong SHOW_DOCUMENT_TYPE           = 0x200;
    static const ulong SHOW_DOCUMENT_FRAGMENT       = 0x400;
    static const ulong SHOW_ALL                     = 0xFFFFFFFF;

public js_properties:
    ext::property<accept_callback_t> accept_node;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_NODE_FILTER_HPP
