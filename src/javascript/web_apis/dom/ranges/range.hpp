#pragma once
#ifndef SBROWSER_RANGE_HPP
#define SBROWSER_RANGE_HPP

#include <dom/ranges/abstract_range.hpp>

namespace dom {
    namespace ranges {class range;}
    namespace nodes {class document_fragment;}
}


class dom::ranges::range : public abstract_range {
public constructors:
    range();

public static_constants:
    static const short START_TO_START = 0;
    static const short START_TO_END = 1;
    static const short END_TO_END = 2;
    static const short END_TO_START = 3;

public methods:
    // dom
    void set_start(nodes::node* node, unsigned long offset);
    void set_start_before(nodes::node* node);
    void set_start_after(nodes::node* node);

    void set_end(nodes::node* node, unsigned long offset);
    void set_end_before(nodes::node* node);
    void set_end_after(nodes::node* node);

    void collapse(bool to_start = false);
    void select_node(nodes::node* node);
    void select_node_contents(nodes::node* node);

    short compare_boundary_points(unsigned short how, ranges::range* source_range);
    nodes::document_fragment* extract_contents();
    nodes::document_fragment* clone_contents();
    nodes::document_fragment* delete_contents();
    nodes::document_fragment* surround_contents();

    void insert_node();
    ranges::range* clone_range();
    short compare_point(nodes::node* node, unsigned long offset);
};


#endif //SBROWSER_RANGE_HPP
