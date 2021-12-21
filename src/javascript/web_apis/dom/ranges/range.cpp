#include "range.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>

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
dom::ranges::range::collapse(
        bool to_start) {

    to_start
            ? end_container = start_container
            : start_container = end_container;
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

    // if the range is collapsed and text based then append a clone of the container to a fragment, replace data
    if (start_container == end_container and (
            ext::property_dynamic_cast<nodes::text*>(start_container)
            or ext::property_dynamic_cast<nodes::processing_instruction*>(start_container)
            or ext::property_dynamic_cast<nodes::comment*>(start_container))) {

        nodes::character_data* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        nodes::character_data* clone = dynamic_cast<nodes::character_data*>(start_container_character_data->clone_node());
        clone->data = helpers::texts::substring_data(start_container_character_data, start_offset, end_offset - start_offset);

        helpers::mutation_algorithms::append(clone, fragment);
        helpers::texts::replace_data(start_container_character_data, start_offset, end_offset - start_offset, "");
        return fragment;
    }

    // get the partially contained and contained children in the range
    auto [first_partially_contained_child, last_partially_contained_child, contained_children] = helpers::ranges::get_range_helper_variables(this, start_container, end_container);
    nodes::node* new_node = nullptr;
    unsigned long new_offset = 0;

    if (helpers::trees::is_ancestor(start_container, end_container)) {
        new_node = start_container;
        new_offset = start_offset;
    }
    else {
        auto start_container_ancestors = helpers::trees::ancestors(start_container);
        auto end_container_ancestors = helpers::trees::ancestors(end_container);

        new_node = start_container_ancestors.intersection(end_container_ancestors).front()->parent_node;
        new_offset = helpers::trees::index(new_node) + 1;
    }

    // handle text changes with the first partially contained node
    if (ext::property_dynamic_cast<nodes::text*>(first_partially_contained_child)
            or ext::property_dynamic_cast<nodes::processing_instruction*>(first_partially_contained_child)
            or ext::property_dynamic_cast<nodes::comment*>(first_partially_contained_child)) {

        nodes::character_data* start_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(start_container);
        nodes::character_data* clone = dynamic_cast<nodes::character_data*>(start_container_character_data->clone_node());
        clone->data = helpers::texts::substring_data(start_container_character_data, start_offset, helpers::trees::length(start_container) - start_offset);

        helpers::mutation_algorithms::append(clone, fragment);
        helpers::texts::replace_data(start_container_character_data, start_offset, helpers::trees::length(start_container) - start_offset, "");
    }
    else {
        nodes::node* clone = first_partially_contained_child->clone_node();
        helpers::mutation_algorithms::append(clone, fragment);

        auto* sub_range = new ranges::range{};
        sub_range->start_container = start_container;
        sub_range->start_offset = start_offset;
        sub_range->end_container = first_partially_contained_child;
        sub_range->end_offset = helpers::trees::length(first_partially_contained_child);

        auto* sub_fragment = sub_range->extract_contents();
        helpers::mutation_algorithms::append(sub_fragment, clone);
    }

    contained_children->for_each([fragment](auto* node) -> void {helpers::mutation_algorithms::append(node, fragment);});

    // handle text changes with the last partially contained node
    if (ext::property_dynamic_cast<nodes::text*>(last_partially_contained_child)
        or ext::property_dynamic_cast<nodes::processing_instruction*>(last_partially_contained_child)
        or ext::property_dynamic_cast<nodes::comment*>(last_partially_contained_child)) {

        nodes::character_data* end_container_character_data = ext::property_dynamic_cast<nodes::character_data*>(end_container);
        nodes::character_data* clone = dynamic_cast<nodes::character_data*>(end_container_character_data->clone_node());
        clone->data = helpers::texts::substring_data(end_container_character_data, 0, end_offset);

        helpers::mutation_algorithms::append(clone, fragment);
        helpers::texts::replace_data(end_container_character_data, 0, end_offset, "");
    }
    else {
        nodes::node* clone = last_partially_contained_child->clone_node();
        helpers::mutation_algorithms::append(clone, fragment);

        auto* sub_range = new ranges::range{};
        sub_range->start_container = last_partially_contained_child;
        sub_range->start_offset = 0;
        sub_range->end_container = end_container;
        sub_range->end_offset = end_offset;

        auto* sub_fragment = sub_range->extract_contents();
        helpers::mutation_algorithms::append(sub_fragment, clone);
    }

    start_container = new_node;
    start_offset = new_offset;
    end_container = new_node;
    end_offset = new_offset;

    return fragment;
}
