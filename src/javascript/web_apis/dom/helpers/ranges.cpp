#include "ranges.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/comment.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/processing_instruction.hpp>
#include <dom/nodes/text.hpp>

#include <dom/ranges/range.hpp>


dom::nodes::node*
dom::helpers::ranges::check_parent_exists(nodes::node* node) {

    helpers::exceptions::throw_v8_exception(
            "node must have a parent",
            INVALID_NODE_TYPE_ERR,
            [node] -> bool {return not node->parent_node;});

    return node->parent_node;
}


bool
dom::helpers::ranges::is_textual_based_range_container(nodes::node* node) {
    return dynamic_cast<nodes::text*>(node)
           or dynamic_cast<nodes::processing_instruction*>(node)
           or dynamic_cast<nodes::comment*>(node);
}


dom::nodes::document_fragment*
dom::helpers::ranges::clone_character_data_and_append(
        nodes::node* node,
        nodes::document_fragment* fragment,
        unsigned long start_offset,
        unsigned long end_offset,
        bool replace) {

    auto* character_data = dynamic_cast<nodes::character_data*>(node);
    auto* clone = dynamic_cast<nodes::character_data*>(character_data->clone_node());
    clone->data = helpers::texts::substring_data(character_data, start_offset, end_offset - start_offset);

    helpers::mutation_algorithms::append(clone, fragment);
    if (replace) helpers::texts::replace_data(character_data, start_offset, end_offset, "");

    return fragment;
}


void
dom::helpers::ranges::append_to_sub_fragment(
        nodes::node* node,
        nodes::document_fragment* fragment,
        nodes::node* start_container,
        nodes::node* end_container,
        unsigned long start_offset,
        unsigned long end_offset) {

    auto* clone = node->clone_node();
    helpers::mutation_algorithms::append(clone, fragment);

    auto* sub_range = new dom::ranges::range{};
    sub_range->start_container = start_container;
    sub_range->start_offset = start_offset;
    sub_range->end_container = node;
    sub_range->end_offset = end_offset;

    auto* sub_fragment = sub_range->extract_contents();
    helpers::mutation_algorithms::append(sub_fragment, clone);
}


std::tuple<dom::nodes::node*, unsigned long>
dom::helpers::ranges::create_new_node_and_offset(
        nodes::node* start_container,
        nodes::node* end_container,
        unsigned long start_offset) {

    auto start_container_ancestors = helpers::trees::ancestors(start_container);
    auto end_container_ancestors = helpers::trees::ancestors(end_container);
    auto* common_ancestor = start_container_ancestors.intersection(end_container_ancestors).front();

    return helpers::trees::is_ancestor(start_container, end_container)
            ? std::make_tuple(start_container, start_offset)
            : std::make_tuple(common_ancestor, helpers::trees::index(common_ancestor) + 1);
}
