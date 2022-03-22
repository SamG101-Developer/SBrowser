#pragma once
#ifndef SBROWSER_RANGE_HPP
#define SBROWSER_RANGE_HPP

#include <dom/ranges/abstract_range.hpp>

namespace dom
{
    namespace helpers {struct range_internals;}
    namespace nodes {class document_fragment;}
    namespace ranges {class range;}
}

namespace geometry::shapes {class dom_rect;}


class dom::ranges::range : public abstract_range
{
public: friends
    friend struct helpers::range_internals;

public: constructors
    range();

public: static_constants
    static const short START_TO_START = 0;
    static const short START_TO_END = 1;
    static const short END_TO_END = 2;
    static const short END_TO_START = 3;

public: methods
    // dom
    auto set_start(nodes::node* node, unsigned long offset) -> void;
    auto set_start_before(nodes::node* node) -> void;
    auto set_start_after(nodes::node* node) -> void;

    auto set_end(nodes::node* node, unsigned long offset) -> void;
    auto set_end_before(nodes::node* node) -> void;
    auto set_end_after(nodes::node* node) -> void;

    auto insert_node(nodes::node* node) -> void;
    auto intersects_node(nodes::node* node) -> bool;
    auto select_node(nodes::node* node) -> void;
    auto select_node_contents(nodes::node* node) -> void;

    auto compare_boundary_points(unsigned short how, ranges::range* source_range) -> short;
    auto compare_point(nodes::node* node, unsigned long offset) -> short;

    auto extract_contents() -> nodes::document_fragment*;
    auto clone_contents() -> nodes::document_fragment*;
    auto delete_contents() -> nodes::document_fragment*;
    auto surround_contents(nodes::node* new_parent) -> nodes::document_fragment*;

    auto collapse(bool to_start = false) -> void;
    auto clone_range() -> ranges::range*;
    auto is_point_in_range(nodes::node* node, unsigned long offset) -> bool;

    auto to_json() -> ext::string;

    // cssom-view TODO into cssom-view mixin?
    // ext::vector<geometry::shapes::dom_rect> get_client_rects();
    // geometry::shapes::dom_rect get_bounding_client_rect();

public properties:
    ext::dom_property<nodes::node*, _F> common_ancestor_container;

public: internal_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private: internal_properties
    nodes::node* m_root;

private accessors:
    auto get_common_ancestor_container() const -> nodes::node*;
};


#endif //SBROWSER_RANGE_HPP
