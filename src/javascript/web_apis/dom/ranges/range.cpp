#include "range.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/text.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/range_internals.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>


dom::ranges::range::range() : abstract_range()
{
    // set the custom accessors
    common_ancestor_container.get = [this] {return get_common_ancestor_container();};

    // set the properties
    start_container << (nodes::node*)javascript::realms::relevant_agent().get<nodes::document*>("associated_document");
    end_container << (nodes::node*)javascript::realms::relevant_agent().get<nodes::document*>("associated_document");
    start_offset << (unsigned long)0;
    end_offset << (unsigned long)0;
}


void
dom::ranges::range::set_start(
        nodes::node* node,
        unsigned long offset)
{
    // if the parent exists, set the start node and offset to (node, offset), using the set_start_or_end helper method
    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, offset, true);
}


void
dom::ranges::range::set_start_before(
        nodes::node* node)
{
    // if the parent exists, set the start node and offset to (node, index of the node - so that the range starts before
    // the node), using the set_start_or_end helper method
    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node), true);
}


void
dom::ranges::range::set_start_after(
        nodes::node* node)
{
    // if the parent exists, set the start node and offset to (node, index of the node + 1 - so that the range starts
    // after the node), using the set_start_or_end helper method
    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node) + 1, true);
}


void
dom::ranges::range::set_end(
        nodes::node* node,
        unsigned long offset)
{
    // if the parent exists, set the end node and offset to (node, offset), using the set_start_or_end helper method
    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, offset, true);
}


void
dom::ranges::range::set_end_before(
        nodes::node* node)
{
    // if the parent exists, set the end node and offset to (node, index of the node - so that the range starts after
    // the node), using the set_start_or_end helper method
    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node), false);
}


void
dom::ranges::range::set_end_after(
        nodes::node* node)
{
    // if the parent exists, set the end node and offset to (node, index of the node + 1 - so that the range starts
    // after the node), using the set_start_or_end helper method
    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node) + 1, false);
}


void
dom::ranges::range::insert_node(
        nodes::node* node) {

    // if the start node is a textually based range container, then throw a hierarchy request error
    helpers::exceptions::throw_v8_exception(
            "start container must not be a text, processing_instruction",
            HIERARCHY_REQUEST_ERR,
            [this] {return helpers::range_internals::is_textual_based_range_container(start_container);});

    // create the pointers for the reference node, parent, the start node cast to a text node, and the new offset
    nodes::node* reference_node;
    nodes::node* parent;
    nodes::text* start_container_text_node = ext::property_dynamic_cast<nodes::text*>(start_container);
    unsigned long new_offset = 0;

    // if the start node is a text node, then set the reference node to the start node, otherwise the start node's child
    // at start_offset position in the child_nodes list
    reference_node = start_container_text_node
            ? start_container
            : start_container->child_nodes->at(start_offset);

    // if the reference node is nullptr, then set the parent to the start node, otherwise the reference_node's parent
    parent = not reference_node
            ? start_container
            : reference_node->parent;

    // ensure that a pre-insertion of node into parent before reference node would be valid
    helpers::mutation_algorithms::ensure_pre_insertion_validity(node, parent, reference_node);

    // if start node is a text node, then set the reference node to a new text node whose data is a substring of the
    // start nodes (text node)'s data (upto offset)
    if (start_container_text_node)
        reference_node = helpers::texts::split(start_container_text_node, start_offset);

    // if the reference node is the node being inserted, then set the reference node to its own next sibling (so that
    // the node being inserted can be inserted in the correct position)
    if (reference_node == node)
        reference_node = reference_node->next_sibling;

    // if the node being inserted has a parent, then remove it from its parent prior to the insertion taking place, as
    // it needs to be pre-inserted into the newly generated parent node
    if (node->parent)
        helpers::mutation_algorithms::remove(node);

    // if the reference node is now nullptr (from the next_sibling instruction), then set the new_offset to the length
    // of the parent (ie the end of it), otherwise the index of the reference node
    new_offset = not reference_node
            ? helpers::trees::length(parent)
            : helpers::trees::index(reference_node);

    // if the node being inserted is a document fragment, then increase the offset by the length of the document
    // fragment (contains fragment of document), otherwise by 1
    new_offset += dynamic_cast<nodes::document_fragment*>(node)
            ? helpers::trees::length(node)
            : 1;

    // pre-insert the node into the parent before the reference node
    helpers::mutation_algorithms::pre_insert(node, parent, reference_node);

    // if the range is collapsed, then set the end container to parent, and the end offset to the new offset
    if (collapsed)
    {
        end_container = parent;
        end_offset = new_offset;
    }
}


bool
dom::ranges::range::intersects_node(
        nodes::node* node)
{
    // if this range's root isn't the same root as the node's root, then return false, as they are both in different
    // trees
    if (m_root != helpers::trees::root(node))
        return false;

    // if the node doesn't have a parent, then return true, as the range will encompass the node
    if (not node->parent)
        return true;

    // save the parent as the node's parent, and the offset to the index of the node
    nodes::node* parent = node->parent;
    unsigned long offset = helpers::trees::index(node);

    // save the parent's node and offset for before and after the parent (parent, offset) and (parent, offset + 1)
    auto* parent_offset_node_0 = parent;
    auto parent_offset_offset_0 = offset;

    auto* parent_offset_node_1 = parent;
    auto parent_offset_offset_1 = offset + 1;

    // return true if the node and offset sit inbetween the start and end containers ie AFTER (start_container,
    // start_offset) and BEFORE (end_container, end_offset)
    return helpers::range_internals::position_relative(parent_offset_node_0, parent_offset_offset_0, end_container, end_offset) == internal::boundary_point_comparison_position::BEFORE
            and helpers::range_internals::position_relative(parent_offset_node_1, parent_offset_offset_1, start_container, start_offset) == internal::boundary_point_comparison_position::AFTER;
}


void
dom::ranges::range::select_node(
        nodes::node* node)
{
    // set the parent to the node's parent (if it exists), and the index to the index of the node
    auto* parent = helpers::range_internals::check_parent_exists(node);
    auto index = helpers::trees::index(node);

    // set the containers to the parent, end the index of the node's index and 1 after the node's index, such that the
    // only thing being selected by the range is the node
    start_container = parent;
    start_offset = index;
    end_container = parent;
    end_offset = index + 1;
}


void
dom::ranges::range::select_node_contents(
        nodes::node* node)
{
    // if the node is a document fragment, then throw an invalid node type error
    helpers::exceptions::throw_v8_exception(
            "node must be a non-document_fragment node",
            INVALID_NODE_TYPE_ERR,
            [node] {return dynamic_cast<nodes::document_fragment*>(node);});

    // set the containers to the node, with the offsets at 0 and the length of the node, such that the only thing being
    // selected is the contents of the node
    start_container = node;
    start_offset = 0;
    end_container = node;
    end_offset = helpers::trees::length(node);
}


short
dom::ranges::range::compare_boundary_points(
        unsigned short how,
        ranges::range* source_range)
{
    // if how isn't a known value, then throw a not supported error
    helpers::exceptions::throw_v8_exception(
            "unknown method of comparison",
            NOT_SUPPORTED_ERR,
            [how] {return not ext::vector<short>{START_TO_START, START_TO_END, END_TO_START, END_TO_END}.contains(how);});

    // if this range's root isn't the same root as the source_ranges's root, then throw a wrong document error
    helpers::exceptions::throw_v8_exception(
            "this range's root must match the source_range's root",
            WRONG_DOCUMENT_ERR,
            [this, source_range] {return m_root != source_range->m_root;});

    // create variables for the containers and offsets
    nodes::node* that_container;
    nodes::node* this_container;
    unsigned short that_offset;
    unsigned short this_offset;

    // determine how the comparison takes place depending on the <how> variable
    switch (how) {

        // start to start is when comparing THAT START to THIS START
        case (START_TO_START):
        {
            std::tie(that_container, that_offset) = std::tie(source_range->start_container, source_range->start_offset);
            std::tie(this_container, this_offset) = std::tie(start_container, start_offset);
            break;
        }

        // start to end is when comparing THAT START to THIS END
        case (START_TO_END):
        {
            std::tie(that_container, that_offset) = std::tie(source_range->start_container, source_range->start_offset);
            std::tie(this_container, this_offset) = std::tie(end_container, end_offset);
            break;
        }

        // end to start is when comparing THAT END to THIS START
        case (END_TO_START):
        {
            std::tie(that_container, that_offset) = std::tie(source_range->end_container, source_range->end_offset);
            std::tie(this_container, this_offset) = std::tie(start_container, start_offset);
            break;
        }

        // end to end is when comparing THAT END to THIS END
        case (END_TO_END):
        {
            std::tie(that_container, that_offset) = std::tie(source_range->end_container, source_range->end_offset);
            std::tie(this_container, this_offset) = std::tie(end_container, end_offset);
            break;
        }
    }

    // determine the comparison value depending on the output of the position_relative helper method
    switch (helpers::range_internals::position_relative(this_container, this_offset, that_container, that_offset))
    {
        // before maps to -1
        case (internal::boundary_point_comparison_position::BEFORE):
            return -1;

        // equals maps to 0
        case (internal::boundary_point_comparison_position::EQUALS):
            return 0;

        // after maps to 1
        case (internal::boundary_point_comparison_position::AFTER):
            return 1;
    }
}


short
dom::ranges::range::compare_point(
        nodes::node* node,
        unsigned long offset)
{
    // if this range's root isn't the same root as the node's root, then throw a wrong document error
    helpers::exceptions::throw_v8_exception(
            "this range must be in the same document as the node",
            WRONG_DOCUMENT_ERR,
            [node, this] {return m_root != helpers::trees::root(node);});

    // if the node is a document fragment node, then throw an invalid node type error
    helpers::exceptions::throw_v8_exception(
            "node must be a non-document_fragment node",
            INVALID_NODE_TYPE_ERR,
            [node] {return dynamic_cast<nodes::document_fragment*>(node);});

    // if the offset is greater than the length of the node, then throw an index size error
    helpers::exceptions::throw_v8_exception(
            "offset must be <= length of the node",
            INDEX_SIZE_ERR,
            [node, offset] {return offset > helpers::trees::length(node);});

    // if the (node, offset) is before the start, then return -1
    if (helpers::range_internals::position_relative(node, offset, start_container, start_offset) == internal::boundary_point_comparison_position::BEFORE)
        return -1;

    // if the (node, offset) is after the end, then return 1
    else if (helpers::range_internals::position_relative(node, offset, end_container, end_offset) == internal::boundary_point_comparison_position::AFTER)
        return 1;

    // otherwise, return 0
    return 0;
}


dom::nodes::document_fragment*
dom::ranges::range::extract_contents()
{
    // create a new document fragment, and if the range is collapsed, then return the document fragment as is (empty)
    auto* fragment = new nodes::document_fragment{};
    if (collapsed)
        return fragment;

    // if the start and end nodes are the same, and are textually based nodes, then return the output of the
    // clone_character_data_and_append helper method
    if (start_container == end_container and helpers::range_internals::is_textual_based_range_container(start_container))
        return helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, end_offset - start_offset, true);

    // extract information from this range and the start and end nodes into a set of variables
    auto&& [first_partially_contained_child, last_partially_contained_child, contained_children] = helpers::range_internals::get_range_helpers_variables(this, start_container, end_container);

    // create a new node and offset and save the information about it (new_node, new_offset)
    auto&& [new_node, new_offset] = helpers::range_internals::create_new_node_and_offset(start_container, end_container, start_offset);

    // if the first partially contained child is a textually based range container, then clone the character data and
    // append it to the fragment, otherwise append the first partially contained child to the fragment
    helpers::range_internals::is_textual_based_range_container(first_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, -start_offset + helpers::trees::length(start_container), true)
            : helpers::range_internals::append_to_sub_fragment(first_partially_contained_child, fragment, start_container, end_container, start_offset, helpers::trees::length(first_partially_contained_child));

    // for each node stored in the contained children, append them to the fragment sequentially
    for (nodes::node* node: contained_children)
        helpers::mutation_algorithms::append(node, fragment);

    // if the first partially contained child is a textually based range container, then clone the character data and
    // append it to the fragment, otherwise append the first partially contained child to the fragment
    helpers::range_internals::is_textual_based_range_container(last_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(end_container, fragment, 0, end_offset, true)
            : helpers::range_internals::append_to_sub_fragment(last_partially_contained_child, fragment, start_container, end_container, 0, end_offset);

    // set the start and end nodes to the new_node, and set the start and end offsets to the new_offset
    start_container = new_node;
    start_offset = new_offset;
    end_container = new_node;
    end_offset = new_offset;

    // return the document fragment
    return fragment;
}


dom::nodes::document_fragment*
dom::ranges::range::clone_contents()
{
    // create a new document fragment, and if the range is collapsed, then return the document fragment as is (empty)
    auto* fragment = new nodes::document_fragment{};
    if (collapsed)
        return fragment;

    // if the start and end nodes are the same, and the start node is a textually based range container, then clone the
    // character data and append it to the fragment
    if (start_container == end_container and helpers::range_internals::is_textual_based_range_container(start_container))
        return helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, end_offset - start_offset, false);

    // extract information from this range and the start and end nodes into a set of variables
    auto [first_partially_contained_child, last_partially_contained_child, contained_children] = helpers::range_internals::get_range_helper_variables(this, start_container, end_container);

    // if the first partially contained child is a textually based range container, then clone the character data and
    // append it to the fragment, otherwise append the first partially contained child to the fragment
    helpers::range_internals::is_textual_based_range_container(first_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, helpers::trees::length(start_container) - start_offset, false)
            : helpers::range_internals::append_to_sub_fragment(first_partially_contained_child, fragment, start_container, first_partially_contained_child, start_offset, helpers::trees::length(first_partially_contained_child));

    // for each node stored in the contained children, append them to the fragment sequentially
    for (nodes::node* node: contained_children)
        helpers::mutation_algorithms::append(node->clone_node(true), fragment);

    // if the first partially contained child is a textually based range container, then clone the character data and
    // append it to the fragment, otherwise append the first partially contained child to the fragment
    helpers::range_internals::is_textual_based_range_container(last_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(start_container, fragment, 0, end_offset, false)
            : helpers::range_internals::append_to_sub_fragment(last_partially_contained_child, fragment, last_partially_contained_child, end_container, 0, end_offset);

    // return the document fragment
    return fragment;
}


dom::nodes::document_fragment*
dom::ranges::range::delete_contents() { // TODO : check this method against the DOM spec (or add replace_data offset info into comments)

    // return nullptr if the range is collapsed
    if (collapsed)
        return nullptr;

    // if the start and end nodes are the same, and the start node is a textually based range container, get the
    // character data cast of the start node, and delete the data in it (replace with "")
    if (start_container == end_container and helpers::range_internals::is_textual_based_range_container(start_container))
    {
        auto* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        helpers::texts::replace_data(start_container_character_data, start_offset, end_offset - start_offset, "");
    }

    // create a new node and offset and save the information about it (new_node, new_offset)
    auto [new_node, new_offset] = helpers::range_internals::create_new_node_and_offset(start_container, end_container, start_offset);

    // the nodes to remove are the all the descendants of this range's root, who are contained by this range, and whose
    // parents aren't contained by this range
    auto nodes_to_remove = helpers::trees::descendants(m_root)
            .filter([this](auto* node) {return helpers::range_internals::contains(node, this);})
            .filter([this](auto* node) {return not helpers::range_internals::contains(node->parent_node, this);});

    // if the start node is a textually based range container, then delete the data in its character data cast
    if (helpers::range_internals::is_textual_based_range_container(start_container)) {
        auto* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        helpers::texts::replace_data(start_container_character_data, start_offset, -start_offset + helpers::trees::length(start_container), "");
    }

    // remove each node in the nodes_to_remove_list
    for (nodes::node* node: nodes_to_remove)
        helpers::mutation_algorithms::remove(node);

    // if the start node is a textually based range container, then delete the data in its character data cast
    if (helpers::range_internals::is_textual_based_range_container(end_container)) {
        auto* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(end_container);
        helpers::texts::replace_data(start_container_character_data, 0, end_offset, "");
    }

    // set the start and end nodes to the new_node, and set the start and end offsets to the new_offset
    start_container = new_node;
    start_offset = new_offset;
    end_container = new_node;
    end_offset = new_offset;
}


dom::nodes::document_fragment*
dom::ranges::range::surround_contents(nodes::node* new_parent)
{
    // if there are text node descendants of this range's root that are partially contained, throw an invalid state error
    helpers::exceptions::throw_v8_exception(
            "cannot surround the contents of a range that partially contains a text node",
            INVALID_STATE_ERR,
            [this] {return not helpers::trees::descendant_text_nodes(m_root).filter([this](auto* node) {return helpers::range_internals::partially_contains(node, this);}).empty();});

    // if the new_parent is a document, document_fragment or document_type, then throw an invalid node type error
    helpers::exceptions::throw_v8_exception(
            "cannot surround the contents of a range with a new parent that is a document, document_fragment or document_type",
            INVALID_NODE_TYPE_ERR,
            [new_parent] {return dynamic_cast<nodes::document*>(new_parent) or dynamic_cast<nodes::document_fragment*>(new_parent) or dynamic_cast<nodes::document_type*>(new_parent);});

    // extract the contents of the range
    auto* fragment = extract_contents();

    // if the new parent has child nodes, then replace them all with nullptr
    if (new_parent->child_nodes)
        helpers::mutation_algorithms::replace_all(nullptr, new_parent);

    // insert the new_parent into the range, append the new parent into the fragment, and select the new_parent in the
    // node
    insert_node(new_parent);
    helpers::mutation_algorithms::append(fragment, new_parent);
    select_node(new_parent);
}


void
dom::ranges::range::collapse(bool to_start)
{
    // if to_start is true, the set the end container to the start container, otherwise the other awayaround
    to_start
            ? end_container = start_container
            : start_container = end_container;
}


dom::ranges::range*
dom::ranges::range::clone_range() {

    // create a new range with the same nodes and offsets as this node
    auto range_object = new range{};
    range_object->start_container = start_container;
    range_object->start_offset = start_offset;
    range_object->end_container = end_container;
    range_object->end_offset = end_offset;

    // return the range
    return range_object;
}


bool
dom::ranges::range::is_point_in_range(
        nodes::node* node,
        unsigned long offset)
{
    // return false if this range's root isn't the same root as the node's root
    if (m_root != helpers::trees::root(node))
        return false;

    // if the node is a document_type node, then throw an invalid node type error
    dom::helpers::exceptions::throw_v8_exception(
            "node cannot be a doctype node",
            INVALID_NODE_TYPE_ERR,
            [node] -> bool {return dynamic_cast<nodes::document_type*>(node);});

    // if the offset > the length of the node, then throw an index error
    dom::helpers::exceptions::throw_v8_exception(
            "offset must be <= the length of the node",
            INDEX_SIZE_ERR,
            [node, offset] {return offset > helpers::trees::length(node);});

    // the point is in the range if it between the start container/offset and end container/offset
    return helpers::range_internals::position_relative(node, offset, start_container, start_offset) == AFTER
            and helpers::range_internals::position_relative(node, offset, end_container, end_offset) == BEFORE;
}


ext::string
dom::ranges::range::to_json() {

    // create the output string, and the start and end containers as text nodes
    ext::string s;
    auto* start_text_node = ext::property_dynamic_cast<nodes::text*>(start_container);
    auto* end_text_node = ext::property_dynamic_cast<nodes::text*>(end_container);

    // if the start and end nodes are the same (and a text node), then return the substring of the text from between the
    // start and end offset
    if (start_container == end_container and start_text_node)
        return start_text_node->data->substring(start_offset, end_offset - start_offset);

    // if the start node is a text node, then increment the output string by the substring of the data in the text node
    // that ranges from the start_offset to the end of the text
    if (start_text_node)
        s += start_text_node->data->substring(start_offset);

    //
    for (auto* descendant_text_node: helpers::trees::descendant_text_nodes(m_root).filter([this](nodes::text* descendant_node) {return helpers::range_internals::contains(descendant_node, this);}))
        s += descendant_text_node->data->substring(start_offset);

    if (end_text_node)
        s += end_text_node->data->substring(0, end_offset);

    return s;
}


dom::nodes::node*
dom::ranges::range::get_common_ancestor_container() const {

    return helpers::trees::common_ancestor(start_container, end_container);
}


ext::any
dom::ranges::range::v8(v8::Isolate* isolate) const {

    return v8pp::class_<range>{isolate}
            .ctor<>()
            .inherit<abstract_range>()

            .static_("START_TO_START", range::START_TO_START)
            .static_("START_TO_END", range::START_TO_END)
            .static_("END_TO_END", range::END_TO_END)
            .static_("END_TO_START", range::END_TO_START)

            .function("setStart", &range::set_start)
            .function("setStartAfter", &range::set_start_after)
            .function("setStartBefore", &range::set_start_before)

            .function("setEnd", &range::set_end)
            .function("setEndAfter", &range::set_end_after)
            .function("setEndBefore", &range::set_end_before)

            .function("insertNode", &range::insert_node)
            .function("intersectsNode", &range::intersects_node)
            .function("selectNode", &range::select_node)
            .function("selectNodeContents", &range::select_node_contents)

            .function("compareBoundaryPoints", &range::compare_boundary_points)
            .function("comparePoint", &range::compare_point)

            .function("extractContents", &range::extract_contents)
            .function("cloneContents", &range::clone_contents)
            .function("deleteContents", &range::delete_contents)
            .function("surroundContents", &range::surround_contents)

            .function("collapse", &range::collapse)
            .function("cloneRange", &range::clone_range)
            .function("isPointInRange", &range::is_point_in_range)

            .function("toJSON", &range::to_json)

            .var("commonAncestorContainer", &range::common_ancestor_container)

            .auto_wrap_objects();
}
