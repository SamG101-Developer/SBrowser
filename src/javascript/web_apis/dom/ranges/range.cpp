#include "range.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/ranges.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>


dom::ranges::range::range() : abstract_range() {
    common_ancestor_container.get = [this] {return get_common_ancestor_container();};

    start_container = (nodes::node*)javascript::realms::relevant_agent().get<nodes::document*>("associated_document");
    end_container = (nodes::node*)javascript::realms::relevant_agent().get<nodes::document*>("associated_document");

    start_offset = 0;
    end_offset = 0;
}


void
dom::ranges::range::set_start(
        nodes::node* node,
        unsigned long offset) {

    helpers::ranges::check_parent_exists(node);
    helpers::ranges::set_start_or_end(this, node, offset, true);
}


void
dom::ranges::range::set_start_before(
        nodes::node* node) {

    helpers::ranges::check_parent_exists(node);
    helpers::ranges::set_start_or_end(this, node, helpers::trees::index(node), true);
}


void
dom::ranges::range::set_start_after(
        nodes::node* node) {

    helpers::ranges::check_parent_exists(node);
    helpers::ranges::set_start_or_end(this, node, helpers::trees::index(node) + 1, true);
}


void
dom::ranges::range::set_end(
        nodes::node* node,
        unsigned long offset) {

    helpers::ranges::check_parent_exists(node);
    helpers::ranges::set_start_or_end(this, node, offset, true);
}


void
dom::ranges::range::set_end_before(
        nodes::node* node) {

    helpers::ranges::check_parent_exists(node);
    helpers::ranges::set_start_or_end(this, node, helpers::trees::index(node), false);
}


void
dom::ranges::range::set_end_after(
        nodes::node* node) {

    helpers::ranges::check_parent_exists(node);
    helpers::ranges::set_start_or_end(this, node, helpers::trees::index(node) + 1, false);
}


void
dom::ranges::range::insert_node(
        nodes::node* node) {

    helpers::exceptions::throw_v8_exception(
            "start container must not be a text, processing_instruction",
            HIERARCHY_REQUEST_ERR,
            [this] -> bool {return helpers::ranges::is_textual_based_range_container(start_container);});

    nodes::node* reference_node;
    nodes::node* parent_node;
    nodes::text* start_container_text = ext::property_dynamic_cast<nodes::text*>(start_container);
    unsigned long new_offset = 0;

    reference_node = start_container_text
            ? start_container
            : start_container->child_nodes->at(start_offset);

    parent_node = not reference_node
            ? start_container
            : reference_node->parent_node;

    helpers::mutation_algorithms::ensure_pre_insertion_validity(node, parent_node, reference_node);
    reference_node = start_container_text
            ? helpers::texts::split(start_container_text, start_offset)
            : reference_node;

    reference_node = reference_node == node
            ? reference_node->next_sibling
            : reference_node;

    if (node->parent_node) helpers::mutation_algorithms::remove(node);
    new_offset = not reference_node
            ? helpers::trees::length(parent_node)
            : helpers::trees::index(reference_node);

    new_offset += dynamic_cast<nodes::document_fragment*>(node)
            ? helpers::trees::length(node)
            : 1;

    helpers::mutation_algorithms::pre_insert(node, parent_node, reference_node);
    if (collapsed)
        end_container = parent_node, end_offset = new_offset;
}


void
dom::ranges::range::select_node(
        nodes::node* node) {

    auto* parent = helpers::ranges::check_parent_exists(node);
    auto index = helpers::trees::index(node);

    start_container = parent;
    start_offset = index;
    end_container = parent;
    end_offset = index + 1;
}


void
dom::ranges::range::select_node_contents(
        nodes::node* node) {

    helpers::exceptions::throw_v8_exception(
            "node must be a non-document_fragment node",
            INVALID_NODE_TYPE_ERR,
            [node] -> bool {return dynamic_cast<nodes::document_fragment*>(node);});

    start_container = node;
    start_offset = 0;
    end_container = node;
    end_offset = helpers::trees::length(node);
}


short
dom::ranges::range::compare_boundary_points(
        unsigned short how,
        ranges::range* source_range) {

    helpers::exceptions::throw_v8_exception(
            "this range's root must match the source_range's root",
            WRONG_DOCUMENT_ERR,
            [this, source_range] -> bool {return m_root == source_range->m_root;});

    nodes::node* that_container;
    nodes::node* this_container;
    unsigned short that_offset;
    unsigned short this_offset;

    switch (how) {
        case (range::START_TO_START): {
            that_container, that_offset = source_range->start_container, source_range->start_offset;
            this_container, this_offset = start_container, start_offset;
            break;
        }

        case (range::START_TO_END): {
            that_container, that_offset = source_range->start_container, source_range->start_offset;
            this_container, this_offset = end_container, end_offset;
            break;
        }

        case (range::END_TO_START): {
            that_container, that_offset = source_range->end_container, source_range->end_offset;
            this_container, this_offset = start_container, start_offset;
            break;
        }

        case (range::END_TO_END): {
            that_container, that_offset = source_range->end_container, source_range->end_offset;
            this_container, this_offset = end_container, end_offset;
        }

        default: {
            helpers::exceptions::throw_v8_exception(
                    "method of comparing boundary points must be in {0, 1, 2, 3}",
                    NOT_SUPPORTED_ERR);
        }
    }

    switch (helpers::ranges::position_relative(this_container, this_offset, that_container, that_offset)) {
        case (internal::boundary_point_comparision_position::BEFORE): {return -1;}
        case (internal::boundary_point_comparision_position::EQUAL): {return 0;}
        case (internal::boundary_point_comparision_position::AFTER): {return 1;}
    }
}


short
dom::ranges::range::compare_point(
        nodes::node* node,
        unsigned long offset) {

    helpers::exceptions::throw_v8_exception(
            "this range must be in the same document as the node",
            WRONG_DOCUMENT_ERR,
            [node, this] -> bool {return m_root != helpers::trees::root(node);});

    helpers::exceptions::throw_v8_exception(
            "node must be a non-document_fragment node",
            INVALID_NODE_TYPE_ERR,
            [node] -> bool {return dynamic_cast<nodes::document_fragment*>(node);});

    helpers::exceptions::throw_v8_exception(
            "offset must be <= length of the node",
            INDEX_SIZE_ERR,
            [node, offset] -> bool {return offset > helpers::trees::length(node);});

    if (helpers::ranges::position_relative(node, offset, start_container, start_offset) == internal::boundary_point_comparidon_position::BEFORE) return -1;
    if (helpers::ranges::position_relative(node, offset, end_container, end_offset) == internal::boundary_point_comparidon_position::AFTER) return 1;
    return 0;
}


dom::nodes::document_fragment*
dom::ranges::range::extract_contents() {
    auto* fragment = new nodes::document_fragment{};
    if (collapsed) return fragment;

    if (start_container == end_container and helpers::ranges::is_textual_based_range_container(start_container))
        return helpers::ranges::clone_character_data_and_append(start_container, fragment, start_offset, end_offset - start_offset, true);

    auto [first_partially_contained_child, last_partially_contained_child, contained_children] = helpers::ranges::get_range_helper_variables(this, start_container, end_container);
    auto [new_node, new_offset] = helpers::ranges::create_new_node_and_offset(start_container, end_container, start_offset);

    helpers::ranges::is_textual_based_range_container(first_partially_contained_child)
            ? helpers::ranges::clone_character_data_and_append(start_container, fragment, start_offset, helpers::trees::length(start_container) - start_offset, true)
            : helpers::ranges::append_to_sub_fragment(first_partially_contained_child, fragment, start_container, end_container, start_offset, helpers::trees::length(first_partially_contained_child));

    contained_children->for_each([fragment](auto* node) -> void {helpers::mutation_algorithms::append(node, fragment);});

    helpers::ranges::is_textual_based_range_container(last_partially_contained_child)
            ? helpers::ranges::clone_character_data_and_append(end_container, fragment, 0, end_offset, true)
            : helpers::ranges::append_to_sub_fragment(last_partially_contained_child, fragment, start_container, end_container, 0, end_offset);

    start_container = new_node, start_offset = new_offset;
    end_container = new_node, end_offset = new_offset;

    return fragment;
}


dom::nodes::document_fragment*
dom::ranges::range::clone_contents() {
    auto* fragment = new nodes::document_fragment{};
    if (collapsed) return fragment;

    if (start_container == end_container and helpers::ranges::is_textual_based_range_container(start_container))
        return helpers::ranges::clone_character_data_and_append(start_container, fragment, start_offset, end_offset - start_offset, false);

    auto [first_partially_contained_child, last_partially_contained_child, contained_children] = helpers::ranges::get_range_helper_variables(this, start_container, end_container);

    helpers::ranges::is_textual_based_range_container(first_partially_contained_child)
            ? helpers::ranges::clone_character_data_and_append(start_container, fragment, start_offset, helpers::trees::length(start_container) - start_offset, false)
            : helpers::ranges::append_to_sub_fragment(first_partially_contained_child, fragment, start_container, first_partially_contained_child, start_offset, helpers::trees::length(first_partially_contained_child));

    contained_children->for_each([fragment](auto* node) -> void {helpers::mutation_algorithms::append(node->clone_node(true), fragment);});

    helpers::ranges::is_textual_based_range_container(last_partially_contained_child)
            ? helpers::ranges::clone_character_data_and_append(start_container, fragment, 0, end_offset, false)
            : helpers::ranges::append_to_sub_fragment(last_partially_contained_child, fragment, last_partially_contained_child, end_container, 0, end_offset);

    return fragment;
}


dom::nodes::document_fragment*
dom::ranges::range::delete_contents() {

    if (collapsed) return nullptr;

    if (start_container == end_container and helpers::ranges::is_textual_based_range_container(start_container)) {
        nodes::character_data* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        helpers::texts::replace_data(start_container_character_data, start_offset, end_offset - start_offset, "");
    }

    auto [new_node, new_offset] = helpers::ranges::create_new_node_and_offset(start_container, end_container, start_offset);
    auto nodes_to_remove = helpers::trees::descendants(m_root)
            .filter([this](auto* node) -> bool {return helpers::ranges::contains(node, this);})
            .filter([this](auto* node) -> bool {return not helpers::ranges::contains(node->parent_node, this);});

    if (helpers::ranges::is_textual_based_range_container(start_container)) {
        nodes::character_data* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        helpers::texts::replace_data(start_container_character_data, start_offset, helpers::trees::length(start_container) - start_offset, "");
    }

    nodes_to_remove.for_each([](auto* node) -> void {helpers::mutation_algorithms::remove(node);});

    if (helpers::ranges::is_textual_based_range_container(end_container)) {
        nodes::character_data* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(end_container);
        helpers::texts::replace_data(start_container_character_data, 0, end_offset, "");
    }

    start_container = new_node, start_offset = new_offset;
    end_container = new_node, end_offset = new_offset;
}


dom::nodes::document_fragment*
dom::ranges::range::surround_contents(
        nodes::node* new_parent) {

    helpers::exceptions::throw_v8_exception(
            "cannot surround the contents of a range that partially contains a text node",
            INVALID_STATE_ERR,
            [this] -> bool {return not helpers::trees::descendants(m_root)
                    .cast_all<nodes::text*>()
                    .filter([](auto* node) -> bool {return helpers::ranges::partially_contains(node, this);})
                    .empty();});

    helpers::exceptions::throw_v8_exception(
            "cannot surround the contents of a range with a new parent that is a document, document_fragment or document_type",
            INVALID_NODE_TYPE_ERR,
            [new_parent] -> bool {return dynamic_cast<nodes::document*>(new_parent)
                    or dynamic_cast<nodes::document_fragment*>(new_parent)
                    or dynamic_cast<nodes::document_type*>(new_parent);});

    auto* fragment = extract_contents();
    if (new_parent->child_nodes) helpers::mutation_algorithms::replace_all(nullptr, new_parent);

    insert_node(new_parent);
    helpers::mutation_algorithms::append(fragment, new_parent);
    select_node(new_parent);
}


void
dom::ranges::range::collapse(
        bool to_start) {

    to_start
    ? end_container = start_container
    : start_container = end_container;
}
