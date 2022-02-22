#pragma once
#ifndef SBROWSER_NODE_FILTER_HPP
#define SBROWSER_NODE_FILTER_HPP

#include <ext/macros/decorators.hpp>
#include <dom_object.hpp>

namespace dom {
    namespace iterators {struct node_filter;}
    namespace nodes {class node;}
}


struct dom::iterators::node_filter : virtual public dom_object {
public: static_constants
    static const unsigned short FILTER_ACCEPT = 1;
    static const unsigned short FILTER_REJECT = 2;
    static const unsigned short FILTER_SKIP = 3;

    static const unsigned long SHOW_ALL = 0xFFFFFFFF;
    static const unsigned long SHOW_ELEMENT = 0x1;
    static const unsigned long SHOW_ATTRIBUTE = 0x2;
    static const unsigned long SHOW_TEXT = 0x4;
    static const unsigned long SHOW_CDATA_SECTION = 0x8;
    static const unsigned long SHOW_PROCESSING_INSTRUCTION = 0x40;
    static const unsigned long SHOW_COMMENT = 0x80;
    static const unsigned long SHOW_DOCUMENT = 0x100;
    static const unsigned long SHOW_DOCUMENT_TYPE = 0x200;
    static const unsigned long SHOW_DOCUMENT_FRAGMENT = 0x400;

public: methods
    auto accept_node(nodes::node* node) const -> unsigned short;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_NODE_FILTER_HPP
