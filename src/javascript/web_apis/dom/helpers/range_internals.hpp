#pragma once
#ifndef SBROWSER_RANGE_INTERNALS_HPP
#define SBROWSER_RANGE_INTERNALS_HPP

#include <tuple>

#include <ext/iterables/vector.hpp>

namespace dom {
    namespace helpers {struct range_internals;}
    namespace internal {enum boundary_point_comparison_position;}
    namespace nodes {
        class character_data;
        class document_fragment;
        class node;
    }
    namespace ranges {class range;}
}


struct dom::helpers::range_internals {
    static bool contains(nodes::node* node, ranges::range* range);
    static bool partially_contains(nodes::node* node, ranges::range* range);
    static void set_start_or_end(ranges::range* range, nodes::node* container, unsigned long offset, bool start);
    static internal::boundary_point_comparison_position position_relative(nodes::node* start_container, unsigned long start_offset, nodes::node* end_container, unsigned long end_offset);

    static std::tuple<nodes::node*, nodes::node*, ext::vector<nodes::node*>> get_range_helpers_variables(ranges::range* range, nodes::node* start_container, nodes::node* end_container);
    static nodes::node* check_parent_exists(nodes::node* node);
    static bool is_textual_based_range_container(nodes::node* node);
    static nodes::document_fragment* clone_character_data_and_append(nodes::node* node, nodes::document_fragment* fragment, unsigned long start_offset, unsigned long end_offset, bool replace);
    static void append_to_sub_fragment(nodes::node* node, nodes::document_fragment* fragment, nodes::node* start_container, nodes::node* end_container, unsigned long start_offset, unsigned long end_offset);
    static std::tuple<nodes::node*, unsigned long> create_new_node_and_offset(nodes::node* start_container, nodes::node* end_container, unsigned long start_offset);
};

enum dom::internal::boundary_point_comparison_position {EQUALS = -1, BEFORE = 0, AFTER = 1};


#endif //SBROWSER_RANGE_INTERNALS_HPP
