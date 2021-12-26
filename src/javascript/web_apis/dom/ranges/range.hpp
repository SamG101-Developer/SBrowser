#pragma once
#ifndef SBROWSER_RANGE_HPP
#define SBROWSER_RANGE_HPP

#include <dom/ranges/abstract_range.hpp>

namespace dom {
    namespace helpers {struct range_internals;}
    namespace nodes {class document_fragment;}
    namespace ranges {class range;}
}

namespace geometry::shapes {class dom_rect;}


class dom::ranges::range : public abstract_range {
friends
    friend struct helpers::range_internals;

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

    void insert_node(nodes::node* node);
    bool intersects_node(nodes::node* node);
    void select_node(nodes::node* node);
    void select_node_contents(nodes::node* node);

    short compare_boundary_points(unsigned short how, ranges::range* source_range);
    short compare_point(nodes::node* node, unsigned long offset);

    nodes::document_fragment* extract_contents();
    nodes::document_fragment* clone_contents();
    nodes::document_fragment* delete_contents();
    nodes::document_fragment* surround_contents(nodes::node* new_parent);

    void collapse(bool to_start = false);
    ranges::range* clone_range();
    bool is_point_in_range(nodes::node* node, unsigned long offset);

    ext::string to_json();

    // cssom-view TODO -> move into cssom-view mixin?
    ext::vector<geometry::shapes::dom_rect> get_client_rects();
    geometry::shapes::dom_rect get_bounding_client_rect();

public properties:
    ext::dom_property<nodes::node*> common_ancestor_container;

private internal_properties:
    nodes::node* m_root;

private accessors:
    nodes::node* get_common_ancestor_container() const;
};


#endif //SBROWSER_RANGE_HPP
