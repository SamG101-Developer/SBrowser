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
        ranges::range* range)
{

    return range->m_root == trees::root(node)
            and position_relative(node, 0, range->start_container, range->start_offset) == internal::AFTER
            and position_relative(node, trees::length(node), range->end_container, range->end_offset) == internal::BEFORE;
}


bool
dom::helpers::range_internals::partially_contains(
        nodes::node* node,
        ranges::range* range)
{

    return trees::is_ancestor(node, range->start_container) and not trees::is_ancestor(node, range->end_container)
            or trees::is_ancestor(node, range->end_container) and not trees::is_ancestor(node, range->start_container);
}


void
dom::helpers::range_internals::set_start_or_end(
        ranges::range* range,
        nodes::node* container,
        unsigned long offset,
        bool start)
{

    exceptions::throw_v8_exception(
            "node must be a non-document_type node",
            INVALID_NODE_TYPE_ERR,
            [container] {return dynamic_cast<nodes::document_type*>(container);});

    exceptions::throw_v8_exception(
            "offset must be <= index of the node",
            INDEX_SIZE_ERR,
            [container, offset] {return offset > trees::index(container);});

    // set the container to the start of the range
    if (start)
    {
        // if the node is in a different tree or indexed after the end of range then set it to the end of the range
        if (range->m_root != trees::root(container) or position_relative(container, offset, range->end_container, range->end_offset) == internal::AFTER)
        {
            range->end_container = container;
            range->end_offset = offset;
        }

        // otherwise, set it to the start of the range
        else
        {
            range->start_container = container;
            range->start_offset = offset;
        }
    }

    // set the container to the end of the range
    else
    {
        // of the node is in a different tree or indexed before the start of the range then set it to the start of the range
        if (range->m_root != trees::root(container) or position_relative(container, offset, range->start_container, range->start_offset) == internal::BEFORE)
        {
            range->start_container = container;
            range->start_offset = offset;
        }

        // otherwise, set it to the end of the range
        else
        {
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
        unsigned long end_offset)
{
    // the start and end container hava to be from the same tree, as a range only covers part of one tree
    assert(trees::root(start_container) == trees::root(end_container));

    // return a comparison on the offsets to compare the position, if the two containers are the same
    if (start_container == end_container)
        return start_offset == end_offset ? internal::EQUALS : start_offset < end_offset ? internal::BEFORE : internal::AFTER;

    // return the opposite of calling the same method with the start and end variables switched, if the start container
    // is following (ie after) the end container opposite
    if (trees::is_following(start_container, end_container))
        return static_cast<internal::boundary_point_comparison_position>(~position_relative(end_container, end_offset, start_container, start_offset));

    // return AFTER if the start container is an ancestor of the end container, and the index of the node that is a
    // child of the start container and an ancestor of the end container is less than the start offset
    if (trees::is_ancestor(start_container, end_container) and trees::index(trees::ancestors(end_container).item_after(start_container)) < start_offset)
        return internal::AFTER;

    // return BEFORE otherwise
    return internal::BEFORE;
}


std::tuple<dom::nodes::node*, dom::nodes::node*, ext::vector<dom::nodes::node*>>
dom::helpers::range_internals::get_range_helpers_variables(
        ranges::range* range,
        nodes::node* start_container,
        nodes::node* end_container)
{
    // get the common ancestor between the start and end container
    auto* common_ancestor = trees::common_ancestor(start_container, end_container);

    // set the first partially contained node to the first child of the common ancestor's children that the range
    // partially contains, if the start container isn't an ancestor of the end container, otherwise set it to nullptr
    nodes::node* first_partially_contained_child = not trees::is_ancestor(start_container, end_container)
            ? common_ancestor->child_nodes->first_match([range](nodes::node* node) {return partially_contains(node, range);})
            : nullptr;

    // set the last partially contained node to the last child of the common ancestor's children that the range#
    // partially container, if the end container isn't an ancestor of the start container, otherwise set it to nullptr
    nodes::node* last_partially_contained_child = not trees::is_ancestor(end_container, start_container)
            ? common_ancestor->child_nodes->last_match([range](nodes::node* node) {return partially_contains(node, range);})
            : nullptr;

    // the contained children are all the children of the common ancestor that are contained by the range
    auto contained_children = common_ancestor->child_nodes->filter([range](nodes::node* node) {return contains(node, range);});

    // if any of the contained_children is a document type, then throw a hierarchy request error TODO : why?
    exceptions::throw_v8_exception(
            "children of the common ancestor of the start and end container of a range must be non-document_type nodes",
            HIERARCHY_REQUEST_ERR,
            [contained_children] {return contained_children.any_of([](nodes::node* contained_child) -> bool {return dynamic_cast<nodes::document_type*>(contained_child);});});

    // return a tuple containing the first and last partially contained nodes, and the contained children
    return std::make_tuple(first_partially_contained_child, last_partially_contained_child, contained_children);
}


dom::nodes::node*
dom::helpers::range_internals::check_parent_exists(nodes::node* node)
{
    // if the node doesn't have a parent, then throw an invalid node error
    exceptions::throw_v8_exception(
            "node must have a parent",
            INVALID_NODE_TYPE_ERR,
            [node] {return not node->parent;});

    // return the parent of the node
    return node->parent;
}


bool
dom::helpers::range_internals::is_textual_based_range_container(nodes::node* node)
{
    // return true if the node cast be cast to a text node, processing instruction or comment
    return multi_cast<nodes::text*, nodes::processing_instruction*, nodes::comment*>(node);
}


dom::nodes::document_fragment*
dom::helpers::range_internals::clone_character_data_and_append(
        nodes::node* node,
        nodes::document_fragment* fragment,
        unsigned long start_offset,
        unsigned long end_offset,
        bool replace)
{
    // cast the node into a character data node, and save a clone of the character data node
    auto* character_data = dynamic_cast<nodes::character_data*>(node);
    auto* clone = dynamic_cast<nodes::character_data*>(character_data->clone_node());

    // set the cloned data to an offset based substring
    clone->data = texts::substring_data(character_data, start_offset, end_offset - start_offset);

    // append the clone to the fragment, and replace the data with an empty string if specified to
    mutation_algorithms::append(clone, fragment);
    if (replace)
        texts::replace_data(character_data, start_offset, end_offset, "");

    // return the fragment
    return fragment;
}


dom::nodes::document_fragment*
dom::helpers::range_internals::append_to_sub_fragment(
        nodes::node* node,
        nodes::document_fragment* fragment,
        nodes::node* start_container,
        nodes::node* end_container,
        unsigned long start_offset,
        unsigned long end_offset)
{
    // save a clone of the node, and append it to the fragment
    auto* clone = node->clone_node();
    mutation_algorithms::append(clone, fragment);

    // create the sub range, ranging from the start to end nodes / offsets
    auto* sub_range = new dom::ranges::range{};
    sub_range->start_container = start_container;
    sub_range->start_offset = start_offset;
    sub_range->end_container = node;
    sub_range->end_offset = end_offset;

    // create the sub_fragment as the sub range's contents that are extracted, and append the clone to the sub_fragment
    auto* sub_fragment = sub_range->extract_contents();
    mutation_algorithms::append(sub_fragment, clone);

    // return the sub_fragment
    return sub_fragment;
}


std::tuple<dom::nodes::node*, unsigned long>
dom::helpers::range_internals::create_new_node_and_offset(
        nodes::node* start_container,
        nodes::node* end_container,
        unsigned long start_offset)
{
    // get the common ancestor between the start and end nodes
    auto* common_ancestor = helpers::trees::common_ancestor(start_container, end_container);

    // return the (start node, start offset) if the start node is an ancestor of the end node, otherwise the
    // (common ancestor, index of the node after the common ancestor - to include the common ancestor)
    return trees::is_ancestor(start_container, end_container)
            ? std::make_tuple(start_container, start_offset)
            : std::make_tuple(common_ancestor, trees::index(common_ancestor) + 1);
}
