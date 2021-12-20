#include "node.hpp"

#include <iostream>

#include <ext/vector.hpp>

#include <javascript/environment/realms.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/shadows.hpp>
#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>

#include <dom/ranges/range.hpp>

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidgetAction>


dom::nodes::node::node() : event_target() {
    base_uri.get = [this] {return get_base_uri();};
    is_connected.get = [this] {return get_is_connected();};
    first_child.get = [this] {return get_first_child();};
    last_child.get = [this] {return get_last_child();};
    previous_sibling.get = [this] {return get_previous_sibling();};
    next_sibling.get = [this] {return get_next_sibling();};

    parent_node.set = [this](auto&& PH1) {set_parent_node(std::forward<decltype(PH1)>(PH1));};
    parent_node << nullptr;
    child_nodes << new ext::vector<node*>{};

    // TODO : set the m_registered_observer_list when the type has been implemented
}


dom::nodes::node::~node() {
    // TODO : m_registered_observer_list->clear(true);
}


void
dom::nodes::node::normalize() const {

    for (text* text_node: helpers::trees::descendant_text_nodes(this)) {
        std::size_t length = helpers::trees::length(text_node);
        if (length <= 0) {
            helpers::mutation_algorithms::remove(text_node);
            continue;
        }

        ext::string data = helpers::trees::contiguous_text_nodes(text_node)
                .transform<ext::string>([](text* contiguous_text_node) -> ext::string {return contiguous_text_node->data;})
                .join("");

        helpers::texts::replace_data(text_node, length, 0, data);
        node* current_node = text_node->next_sibling;

        auto* live_ranges = javascript::realms::surrounding_agent().get<ext::vector<ranges::range*>*>("live_ranges");
        while (helpers::trees::is_exclusive_text_node(current_node)) {
            live_ranges
                    ->filter([current_node](auto* range) -> bool {return range->start_container == current_node;})
                    .for_each([text_node](auto* range) -> void {range->start_offset += length; range->start_container = text_node;});

            live_ranges
                    ->filter([current_node](auto* range) -> bool {return range->end_container == current_node;})
                    .for_each([text_node](auto* range) -> void {range->end_offset += length; range->end_container = text_node;});

            live_ranges
                    ->filter([current_node](auto* range) -> bool {return range->start_container == current_node->parent_node;})
                    .for_each([length, text_node](auto* range) -> void {range->start_offset = length; range->start_container = text_node;});

            live_ranges
                    ->filter([current_node](auto* range) -> bool {return range->end_container == current_node->parent_node;})
                    .for_each([length, text_node](auto* range) -> void {range->end_offset = length; range->end_container = text_node;});

            length += helpers::trees::length(current_node);
            current_node = current_node->next_sibling;
        }
    }

    for (text* text_node: helpers::trees::contiguous_text_nodes(this))
        helpers::mutation_algorithms::remove(text_node);
}


bool dom::nodes::node::has_child_nodes() const {return child_nodes->empty();}

bool dom::nodes::node::contains(node* other) const {return helpers::trees::is_descendant(other, this);}

bool dom::nodes::node::is_equal_node(node* other) const {return other and helpers::node_internals::equals(this, other);}

bool dom::nodes::node::is_default_namespace(ext::cstring& namespace_) const {return namespace_ == helpers::node_internals::locate_a_namespace(this, "");}


ext::string
dom::nodes::node::lookup_prefix(
        ext::cstring& namespace_) const {

    if (auto* element_node = dynamic_cast<const element*>(this))
        return helpers::node_internals::locate_a_namespace_prefix(element_node, namespace_);

    if (auto* document_node = dynamic_cast<const document*>(this))
        return helpers::node_internals::locate_a_namespace_prefix(document_node->document_element, namespace_);

    if (auto* document_type_node = dynamic_cast<const document_type*>(this))
        return nullptr;

    if (auto* document_fragment_node = dynamic_cast<const document_fragment*>(this))
        return nullptr;

    if (auto* attribute_node = dynamic_cast<const attr*>(this))
        return helpers::node_internals::locate_a_namespace_prefix(attribute_node->owner_element, namespace_);

    return parent_element
            ? helpers::node_internals::locate_a_namespace_prefix(parent_element, namespace_)
            : nullptr;
}


ext::string dom::nodes::node::lookup_namespace_uri(ext::cstring& prefix) const {return helpers::node_internals::locate_a_namespace(this, prefix);}

unsigned short
dom::nodes::node::compare_document_position(node* other) const {
    if (this == other) return 0;

    node* node_1 = other;
    node* node_2 = this;
    attr* attr_1 = nullptr;
    attr* attr_2 = nullptr;

    element* node_1_as_element = nullptr;
    element* node_2_as_element = nullptr;

    // if other is an attr -> set attr_1 to the attribute and node_1 to the owner element
    if (dynamic_cast<const attr*>(node_1)) {
        const node* temp = node_1;
        node_1_as_element = attr_1->owner_element;
        attr_1 = (attr*)dynamic_cast<const attr*>(temp);
    }

    // if this is an attr -> set attr_2 to the attribute and node_1 to the owner element
    if (dynamic_cast<const attr*>(node_2)) {
        const node* temp = node_2;
        node_2_as_element = attr_2->owner_element;
        attr_2 = (attr*)dynamic_cast<const attr*>(temp);
    }

    // if both nodes are attributes with the same owner element
    if (attr_1 and attr_2 and attr_1 == attr_2 and node_1_as_element == node_2_as_element) {
        for (attr* attribute: *node_2_as_element->attributes) {
            if (attribute == attr_1) return DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC | DOCUMENT_POSITION_PRECEDING;
            if (attribute == attr_2) return DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC | DOCUMENT_POSITION_FOLLOWING;
        }
    }

    // disconnected when: an attribute node doesn't have an owner element, other is null, or nodes in different trees
    if (not node_1
            or (attr_1 and not node_1_as_element or attr_2 and not node_2_as_element)
            or helpers::trees::root(node_1) != helpers::trees::root(node_2)) {

        return DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC
                | DOCUMENT_POSITION_DISCONNECTED
                | (node_1 < node_2 ? DOCUMENT_POSITION_PRECEDING : DOCUMENT_POSITION_FOLLOWING);
    }

    // other contains, preceding when: other is an ancestor (not an attr), or other is the owner element of this (attr)
    if (not attr_1 and helpers::trees::is_ancestor(node_1, node_2) or attr_2->owner_element == node_1)
        return DOCUMENT_POSITION_CONTAINS | DOCUMENT_POSITION_PRECEDING;

    // other contained, following when: this is an ancestor (not an attr), or this is the owner element of other (attr)
    if (not attr_2 and helpers::trees::is_ancestor(node_2, node_1) or attr_1->owner_element == node_2)
        return DOCUMENT_POSITION_CONTAINED_BY | DOCUMENT_POSITION_FOLLOWING;

    // other preceding or following depends on index comparison (in the same tree)
    return helpers::trees::is_preceding(node_1, node_2)
            ? DOCUMENT_POSITION_PRECEDING
            : DOCUMENT_POSITION_FOLLOWING
}


dom::nodes::node*
dom::nodes::node::get_root_node(ext::cstring_any_map& options) const {

    return ext::any_cast<bool>(options.at("composed"))
            ? helpers::shadows::shadow_including_root(this)
            : helpers::trees::root(this);
}


dom::nodes::node*
dom::nodes::node::clone_node(bool deep) const {

    helpers::exceptions::throw_v8_exception(
            "cannot clone a shadow root node",
            helpers::exceptions::NOT_SUPPORTED_ERR,
            [this] -> bool {return helpers::shadows::is_shadow_root(this);});

    return helpers::node_internals::clone(this, nullptr, deep);
}


dom::nodes::node*
dom::nodes::node::insert_before(
        node* new_node,
        node* child) {

    ce_reactions(&node::insert_before);
    helpers::mutation_algorithms::pre_insert(new_node, this, child);
}


dom::nodes::node*
dom::nodes::node::append_child(
        node* new_node) {

    ce_reactions(&node::append_child);
    helpers::mutation_algorithms::append(new_node, this);
}


dom::nodes::node*
dom::nodes::node::replace_child(
        node* old_node,
        node* new_node) {

    ce_reactions(&node::replace_child);
    helpers::mutation_algorithms::replace(new_node, old_node, this);
}


dom::nodes::node*
dom::nodes::node::remove_child(
        node* old_node) {

    ce_reactions(&node::remove_child);
    helpers::mutation_algorithms::pre_remove(child, this);
}


ext::string dom::nodes::node::get_node_value() const {return "";}
ext::string dom::nodes::node::get_text_content() const {return "";}

bool dom::nodes::node::get_is_connected() const {return helpers::shadows::is_connected(this);}
ext::string dom::nodes::node::get_base_uri() const {return url::helpers::serializing::serialize_url(owner_document->base_uri);}
dom::nodes::node* dom::nodes::node::get_first_child() const {return child_nodes->front();}
dom::nodes::node* dom::nodes::node::get_last_child() const {return child_nodes->back();}
dom::nodes::node* dom::nodes::node::get_previous_sibling() const {return parent_node->child_nodes->item_before(this);}
dom::nodes::node* dom::nodes::node::get_next_sibling() const {return parent_node->child_nodes->item_after(this);}
dom::nodes::element* dom::nodes::node::get_parent_element() const {return ext::property_dynamic_cast<element*>(parent_node);}

void dom::nodes::node::set_parent_node(node* val) {

    if (parent_node and parent_element->child_nodes->contains(this))
        parent_node->child_nodes->remove(this);

    parent_node << val;
    parent_node->child_nodes->append(this);

    if (m_rendered_widget->isWidgetType()) {

        if (auto* new_parent_scroll_widget = dynamic_cast<QScrollArea*>(val->render())) {
            auto* this_widget = dynamic_cast<QWidget*>(m_rendered_widget);
            new_parent_scroll_widget->widget()->layout()->addWidget(this_widget);
        }

        else if (auto new_parent_action_widget = dynamic_cast<QWidgetAction*>(val->render())) {
            auto* this_widget = dynamic_cast<QWidget*>(m_rendered_widget);
            new_parent_action_widget->setDefaultWidget(this_widget);
        }

        else if (auto* new_parent_widget = dynamic_cast<QWidget*>(val->render())) {
            auto* this_widget = dynamic_cast<QWidget*>(m_rendered_widget);
            new_parent_widget->layout()->addWidget(this_widget);
        }

        else {
            std::cout << "error -> unknown widget type trying to render" << std::endl;
            delete m_rendered_widget;
            return;
        }

        m_rendered_widget->show();
    }
}


bool dom::nodes::node::equals(nodes::node* other) {
    if (child_nodes->length() != other->child_nodes->length()) return false;
    for (std::size_t child_index = 0; child_index < child_nodes->length(); ++child_index) {
        if (not child_nodes->at(child_index)->equals(other->child_nodes->at(child_index))) return false;
    }
    return true;
}
