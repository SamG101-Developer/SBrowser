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

    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, offset, true);
}


void
dom::ranges::range::set_start_before(
        nodes::node* node) {

    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node), true);
}


void
dom::ranges::range::set_start_after(
        nodes::node* node) {

    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node) + 1, true);
}


void
dom::ranges::range::set_end(
        nodes::node* node,
        unsigned long offset) {

    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, offset, true);
}


void
dom::ranges::range::set_end_before(
        nodes::node* node) {

    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node), false);
}


void
dom::ranges::range::set_end_after(
        nodes::node* node) {

    helpers::range_internals::check_parent_exists(node);
    helpers::range_internals::set_start_or_end(this, node, helpers::trees::index(node) + 1, false);
}


void
dom::ranges::range::insert_node(
        nodes::node* node) {

    helpers::exceptions::throw_v8_exception(
            "start container must not be a text, processing_instruction",
            HIERARCHY_REQUEST_ERR,
            [this] {return helpers::ranges::is_textual_based_range_container(start_container);});

    nodes::node* reference_node;
    nodes::node* parent;
    nodes::text* start_container_text = ext::property_dynamic_cast<nodes::text*>(start_container);
    unsigned long new_offset = 0;

    reference_node = start_container_text
            ? start_container
            : start_container->child_nodes->at(start_offset);

    parent = not reference_node
            ? start_container
            : reference_node->parent;

    helpers::mutation_algorithms::ensure_pre_insertion_validity(node, parent, reference_node);
    reference_node = start_container_text
            ? helpers::texts::split(start_container_text, start_offset)
            : reference_node;

    reference_node = reference_node == node
            ? reference_node->next_sibling
            : reference_node;

    if (node->parent) helpers::mutation_algorithms::remove(node);
    new_offset = not reference_node
            ? helpers::trees::length(parent)
            : helpers::trees::index(reference_node);

    new_offset += dynamic_cast<nodes::document_fragment*>(node)
            ? helpers::trees::length(node)
            : 1;

    helpers::mutation_algorithms::pre_insert(node, parent, reference_node);
    if (collapsed)
        end_container = parent, end_offset = new_offset;
}


bool
dom::ranges::range::intersects_node(
        nodes::node* node) {

    if (m_root != helpers::trees::root(node)) return false;
    if (not node->parent) return true;

    nodes::node* parent = node->parent;
    unsigned long offset = helpers::trees::index(node);

    auto* parent_offset_node_0 = parent;
    auto parent_offset_offset_0 = offset;

    auto* parent_offset_node_1 = parent;
    auto parent_offset_offset_1 = offset + 1;

    return helpers::range_internals::position_relative(parent_offset_node_0, parent_offset_offset_0, end_container, end_offset) == internal::boundary_point_comparision_position::BEFORE
            and helpers::range_internals::position_relative(parent_offset_node_1, parent_offset_offset_1, start_container, start_offset) == internal::boundary_point_comparision_position::AFTER
}


void
dom::ranges::range::select_node(
        nodes::node* node) {

    auto* parent = helpers::range_internals::check_parent_exists(node);
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
            [node] {return dynamic_cast<nodes::document_fragment*>(node);});

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
            [this, source_range] {return m_root == source_range->m_root;});

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

    switch (helpers::range_internals::position_relative(this_container, this_offset, that_container, that_offset)) {
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
            [node, this] {return m_root != helpers::trees::root(node);});

    helpers::exceptions::throw_v8_exception(
            "node must be a non-document_fragment node",
            INVALID_NODE_TYPE_ERR,
            [node] {return dynamic_cast<nodes::document_fragment*>(node);});

    helpers::exceptions::throw_v8_exception(
            "offset must be <= length of the node",
            INDEX_SIZE_ERR,
            [node, offset] {return offset > helpers::trees::length(node);});

    if (helpers::range_internals::position_relative(node, offset, start_container, start_offset) == internal::boundary_point_comparidon_position::BEFORE) return -1;
    if (helpers::range_internals::position_relative(node, offset, end_container, end_offset) == internal::boundary_point_comparidon_position::AFTER) return 1;
    return 0;
}


dom::nodes::document_fragment*
dom::ranges::range::extract_contents() {
    auto* fragment = new nodes::document_fragment{};
    if (collapsed) return fragment;

    if (start_container == end_container and helpers::range_internals::is_textual_based_range_container(start_container))
        return helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, end_offset - start_offset, true);

    auto [first_partially_contained_child, last_partially_contained_child, contained_children] = helpers::range_internals::get_range_helper_variables(this, start_container, end_container);
    auto [new_node, new_offset] = helpers::range_internals::create_new_node_and_offset(start_container, end_container, start_offset);

    helpers::range_internals::is_textual_based_range_container(first_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, helpers::trees::length(start_container) - start_offset, true)
            : helpers::range_internals::append_to_sub_fragment(first_partially_contained_child, fragment, start_container, end_container, start_offset, helpers::trees::length(first_partially_contained_child));

    contained_children->for_each([fragment](auto* node) {helpers::mutation_algorithms::append(node, fragment);});

    helpers::range_internals::is_textual_based_range_container(last_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(end_container, fragment, 0, end_offset, true)
            : helpers::range_internals::append_to_sub_fragment(last_partially_contained_child, fragment, start_container, end_container, 0, end_offset);

    start_container = new_node, start_offset = new_offset;
    end_container = new_node, end_offset = new_offset;

    return fragment;
}


dom::nodes::document_fragment*
dom::ranges::range::clone_contents() {
    auto* fragment = new nodes::document_fragment{};
    if (collapsed) return fragment;

    if (start_container == end_container and helpers::range_internals::is_textual_based_range_container(start_container))
        return helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, end_offset - start_offset, false);

    auto [first_partially_contained_child, last_partially_contained_child, contained_children] = helpers::range_internals::get_range_helper_variables(this, start_container, end_container);

    helpers::range_internals::is_textual_based_range_container(first_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(start_container, fragment, start_offset, helpers::trees::length(start_container) - start_offset, false)
            : helpers::range_internals::append_to_sub_fragment(first_partially_contained_child, fragment, start_container, first_partially_contained_child, start_offset, helpers::trees::length(first_partially_contained_child));

    contained_children->for_each([fragment](auto* node) {helpers::mutation_algorithms::append(node->clone_node(true), fragment);});

    helpers::range_internals::is_textual_based_range_container(last_partially_contained_child)
            ? helpers::range_internals::clone_character_data_and_append(start_container, fragment, 0, end_offset, false)
            : helpers::range_internals::append_to_sub_fragment(last_partially_contained_child, fragment, last_partially_contained_child, end_container, 0, end_offset);

    return fragment;
}


dom::nodes::document_fragment*
dom::ranges::range::delete_contents() {

    if (collapsed) return nullptr;

    if (start_container == end_container and helpers::range_internals::is_textual_based_range_container(start_container)) {
        nodes::character_data* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        helpers::texts::replace_data(start_container_character_data, start_offset, end_offset - start_offset, "");
    }

    auto [new_node, new_offset] = helpers::range_internals::create_new_node_and_offset(start_container, end_container, start_offset);
    auto nodes_to_remove = helpers::trees::descendants(m_root)
            .filter([this](auto* node) {return helpers::range_internals::contains(node, this);})
            .filter([this](auto* node) {return not helpers::range_internals::contains(node->parent_node, this);});

    if (helpers::range_internals::is_textual_based_range_container(start_container)) {
        nodes::character_data* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        helpers::texts::replace_data(start_container_character_data, start_offset, helpers::trees::length(start_container) - start_offset, "");
    }

    nodes_to_remove.for_each([](auto* node) {helpers::mutation_algorithms::remove(node);});

    if (helpers::range_internals::is_textual_based_range_container(end_container)) {
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
            [this] {return not helpers::trees::descendants(m_root)
                    .cast_all<nodes::text*>()
                    .filter([](auto* node) {return helpers::range_internals::partially_contains(node, this);})
                    .empty();});

    helpers::exceptions::throw_v8_exception(
            "cannot surround the contents of a range with a new parent that is a document, document_fragment or document_type",
            INVALID_NODE_TYPE_ERR,
            [new_parent] {return dynamic_cast<nodes::document*>(new_parent)
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


dom::ranges::range*
dom::ranges::range::clone_range() {

    auto range_object = new range{};
    range_object->start_container = start_container;
    range_object->start_offset = start_offset;
    range_object->end_container = end_container;
    range_object->end_offset = end_offset;
    return range_object;
}


bool
dom::ranges::range::is_point_in_range(
        nodes::node* node,
        unsigned long offset) {

    if (m_root != helpers::trees::root(node)) return false;
}


ext::string
dom::ranges::range::to_json() {

    ext::string s;
    nodes::text* start_text_node = ext::property_dynamic_cast<nodes::text*>(start_container);
    nodes::text* end_text_node = ext::property_dynamic_cast<nodes::text*>(end_container);

    if (start_container == end_container and start_text_node)
        return start_text_node->data->substring(start_offset, end_offset - start_offset);

    if (start_text_node)
        s += start_text_node->data->substring(start_offset);

    helpers::trees::descendants(m_root)
            .filter([this](auto* descendant_node) {return helpers::range_internals::contains(descendant_node, this);})
            .cast_all<nodes::text*>()
            .for_each([&s, this](auto* descendant_node) {s += descendant_node->data->substring(start_offset);});

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
