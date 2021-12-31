#include "range_internals.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/comment.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/processing_instruction.hpp>
#include <dom/nodes/text.hpp>

#include <dom/ranges/range.hpp>


bool
dom::helpers::range_internals::contains(
        nodes::node* node,
        ranges::range* range) {

    return range->m_root == trees::root(node)
            and position_relative(node, 0, range->start_container, range->start_offset) == internal::AFTER
            and position_relative(node, trees::length(node), range->end_container, range->end_offset) == BEFORE;
}


bool
dom::helpers::range_internals::partially_contains(
        nodes::node* node,
        ranges::range* range) {

    return trees::is_ancestor(node, range->start_container) and not trees::is_ancestor(node, range->end_container)
            or trees::is_ancestor(node, range->end_container) and not trees::is_ancestor(node, range->start_container);
}


void
dom::helpers::range_internals::set_start_or_end(
        ranges::range* range,
        nodes::node* container,
        unsigned long offset,
        bool start) {

    exceptions::throw_v8_exception(
            "node must be a non-document_type node",
            INVALID_NODE_TYPE_ERR,
            [container] {return dynamic_cast<nodes::document_type*>(container);});

    exceptions::throw_v8_exception(
            "offset must be <= index of the node",
            INDEX_SIZE_ERR,
            [container, offset] {return offset > trees::index(container);});

    // set the container to the start of the range
    if (start) {

        // if the node is in a different tree or indexed after the end of range then set it to the end of the range
        if (range->m_root != trees::root(container) or position_relative(container, offset, range->end_container, range->end_offset) == AFTER) {
            range->end_container = container;
            range->end_offset = offset;
        }

        // otherwise, set it to the start of the range
        else {
            range->start_container = container;
            range->start_offset = offset;
        }
    }

    // set the container to the end of the range
    else {

        // of the node is in a different tree or indexed before the start of the range then set it to the start of the range
        if (range->m_root != trees::root(container) or position_relative(container, offset, range->start_container, range->start_offset) == BEFORE) {
            range->start_container = container;
            range->start_offset = offset;
        }

        // otherwise, set it to the end of the range
        else {
            range->end_container = container;
            range->end_offset = offset;
        }
    }
}


dom::internal::boundary_point_comparison_position
dom::helpers::range_internals::position_relative(
        nodes::node* start_container,
        unsigned long start_offset,
        nodes::node* end_container,
        unsigned long end_offset) {

    assert(trees::root(start_container) == trees::root(end_container));

    if (start_container == end_container)
        return start_offset == end_offset ? EQUALS : start_offset < end_offset ? BEFORE : AFTER;

    if (trees::is_following(start_container, end_container))
        return static_cast<internal::boundary_point_comparison_position>(~position_relative(end_container, end_offset, start_container, start_offset));

    if (trees::is_ancestor(start_container, end_container) and trees::index(trees::ancestors(end_container).item_after(start_container)) < start_offset)
        return AFTER;

    return BEFORE;
}


std::tuple<dom::nodes::node*, dom::nodes::node*, ext::vector<dom::nodes::node*>>
dom::helpers::range_internals::get_range_helpers_variables(
        ranges::range* range,
        nodes::node* start_container,
        nodes::node* end_container) {

    auto* common_ancestor = trees::common_ancestor(start_container, end_container);

    nodes::node* first_partially_contained_child = not trees::is_ancestor(start_container, end_container)
            ? common_ancestor->child_nodes->filter([range](auto* node) {return partially_contains(node, range);}).front()
            : nullptr;

    nodes::node* last_partially_contained_child = not trees::is_ancestor(end_container, start_container)
            ? common_ancestor->child_nodes->filter([range](auto* node) {return partially_contains(node, range);}).back()
            : nullptr;

    auto contained_children = common_ancestor->child_nodes->filter([range](auto* node) {return contains(node, range);});

    exceptions::throw_v8_exception(
            "children of the common ancestor of the start and end container of a range must be non-document_type nodes",
            HIERARCHY_REQUEST_ERR,
            [contained_children] {return contained_children.any_of([](auto* contained_child) {return dynamic_cast<nodes::document_type*>(contained_child);});});

    return std::make_tuple(first_partially_contained_child, last_partially_contained_child, contained_children);
}


dom::nodes::node*
dom::helpers::range_internals::check_parent_exists(nodes::node* node) {

    exceptions::throw_v8_exception(
            "node must have a parent",
            INVALID_NODE_TYPE_ERR,
            [node] {return not node->parent;});

    return node->parent;
}


bool
dom::helpers::range_internals::is_textual_based_range_container(nodes::node* node) {
    return dynamic_cast<nodes::text*>(node)
           or dynamic_cast<nodes::processing_instruction*>(node)
           or dynamic_cast<nodes::comment*>(node);
}


dom::nodes::document_fragment*
dom::helpers::range_internals::clone_character_data_and_append(
        nodes::node* node,
        nodes::document_fragment* fragment,
        unsigned long start_offset,
        unsigned long end_offset,
        bool replace) {

    auto* character_data = dynamic_cast<nodes::character_data*>(node);
    auto* clone = dynamic_cast<nodes::character_data*>(character_data->clone_node());
    clone->data = texts::substring_data(character_data, start_offset, end_offset - start_offset);

    mutation_algorithms::append(clone, fragment);
    if (replace) texts::replace_data(character_data, start_offset, end_offset, "");

    return fragment;
}


void
dom::helpers::range_internals::append_to_sub_fragment(
        nodes::node* node,
        nodes::document_fragment* fragment,
        nodes::node* start_container,
        nodes::node* end_container,
        unsigned long start_offset,
        unsigned long end_offset) {

    auto* clone = node->clone_node();
    mutation_algorithms::append(clone, fragment);

    auto* sub_range = new dom::ranges::range{};
    sub_range->start_container = start_container;
    sub_range->start_offset = start_offset;
    sub_range->end_container = node;
    sub_range->end_offset = end_offset;

    auto* sub_fragment = sub_range->extract_contents();
    mutation_algorithms::append(sub_fragment, clone);
}


std::tuple<dom::nodes::node*, unsigned long>
dom::helpers::range_internals::create_new_node_and_offset(
        nodes::node* start_container,
        nodes::node* end_container,
        unsigned long start_offset) {

    auto* common_ancestor = helpers::trees::common_ancestor(start_container, end_container);

    return trees::is_ancestor(start_container, end_container)
            ? std::make_tuple(start_container, start_offset)
            : std::make_tuple(common_ancestor, trees::index(common_ancestor) + 1);
}
