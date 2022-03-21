#pragma once
#ifndef SBROWSER_RANGE_INTERNALS_HPP
#define SBROWSER_RANGE_INTERNALS_HPP

#include <tuple>

#include <ext/vector.hpp>

namespace dom
{
    namespace helpers {struct range_internals;}
    namespace internal {enum class boundary_point_comparison_position;}
    namespace nodes
    {
        class character_data;
        class document_fragment;
        class node;
    }
    namespace ranges {class range;}
}


/*
 * Group of helper methods designed to help with the internal behaviour of ranges, such as (partially) containing nodes,
 * setting the start or end of the range, and interacting fragments with ranges.
 */
struct dom::helpers::range_internals
{
    static auto contains(const nodes::node* node, const ranges::range* range) -> bool;
    static auto partially_contains(const nodes::node* node, const ranges::range* range) -> bool;
    static auto set_start_or_end(ranges::range* range, nodes::node* container, unsigned long offset, bool start) -> void;
    static auto position_relative(nodes::node* start_container, unsigned long start_offset, nodes::node* end_container, unsigned long end_offset) -> internal::boundary_point_comparison_position;

    static auto get_range_helpers_variables(ranges::range* range, const nodes::node* start_container, const nodes::node* end_container) -> std::tuple<nodes::node*, nodes::node*, ext::vector<nodes::node*>>;
    static auto check_parent_exists(const nodes::node* node) -> nodes::node*;
    static auto is_textual_based_range_container(const nodes::node* node) -> bool;
    static auto clone_character_data_and_append(nodes::node* node, nodes::document_fragment* fragment, unsigned long start_offset, unsigned long end_offset, bool replace) -> nodes::document_fragment*;
    static auto append_to_sub_fragment(nodes::node* node, nodes::document_fragment* fragment, nodes::node* start_container, nodes::node* end_container, unsigned long start_offset, unsigned long end_offset) -> nodes::document_fragment*;
    static auto create_new_node_and_offset(const nodes::node* start_container, const nodes::node* end_container, const unsigned long start_offset) -> std::tuple<nodes::node*, unsigned long>;
};


/*
 * Enumeration for comparing the boundary points of two ranges
 */
enum class dom::internal::boundary_point_comparison_position
{
    EQUALS = -1,
    BEFORE = 0,
    AFTER = 1
};


#endif //SBROWSER_RANGE_INTERNALS_HPP
