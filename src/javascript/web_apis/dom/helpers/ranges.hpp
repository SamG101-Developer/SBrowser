#pragma once
#ifndef SBROWSER_RANGES_HPP
#define SBROWSER_RANGES_HPP

#include <tuple>

namespace dom {
    namespace helpers {struct ranges;}
    namespace nodes {
        class character_data;
        class document_fragment;
        class node;
    }
}


struct dom::helpers::ranges {
    static nodes::node* check_parent_exists(nodes::node* node);
    static bool is_textual_based_range_container(nodes::node* node);
    static nodes::document_fragment* clone_character_data_and_append(nodes::node* node, nodes::document_fragment* fragment, unsigned long start_offset, unsigned long end_offset, bool replace);
    static void append_to_sub_fragment(nodes::node* node, nodes::document_fragment* fragment, nodes::node* start_container, nodes::node* end_container, unsigned long start_offset, unsigned long end_offset);
    static std::tuple<nodes::node*, unsigned long> create_new_node_and_offset(nodes::node* start_container, nodes::node* end_container, unsigned long start_offset);
};


#endif //SBROWSER_RANGES_HPP
