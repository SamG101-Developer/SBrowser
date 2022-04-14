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
#include <QtWidgets/QLayout>


dom::nodes::node::node() : event_target()
{
    // set the custom accessor methods
    base_uri.getter         = [this] {return get_base_uri();};
    is_connected.getter     = [this] {return get_is_connected();};
    first_child.getter      = [this] {return get_first_child();};
    last_child.getter       = [this] {return get_last_child();};
    previous_sibling.getter = [this] {return get_previous_sibling();};
    next_sibling.getter     = [this] {return get_next_sibling();};

    parent.setter = [this](auto&& PH1) {set_parent_node(std::forward<decltype(PH1)>(PH1));};

    // set the property values
    parent      << nullptr;
    child_nodes << new ext::vector<node*>{};

    // TODO : set the m_registered_observer_list when the type has been implemented
}


dom::nodes::node::~node()
{
    // TODO : m_registered_observer_list->clear(true);
}


auto dom::nodes::node::normalize() const
        -> void
{
    // iterator over the text nodes that are descendants of this node
    for (const text* text_node: helpers::trees::descendant_text_nodes(this))
    {
        // if the length of the text node (ie the length of the text) is 0 then remove the text node
        size_t length = helpers::trees::length(text_node);
        if (length <= 0)
        {
            helpers::mutation_algorithms::remove(text_node);
            continue;
        }

        // combine the text from the next consecutive text nodes into the text node TODO : remove '(const node*)'
        ext::string data = helpers::trees::contiguous_text_nodes((const node*)text_node)
                .transform<ext::string>([](text* contiguous_text_node) -> ext::string {return contiguous_text_node->data;})
                .join(EMPTY_CHAR);

        // replace the data in this node with the combined data from the contiguous nodes
        helpers::texts::replace_data(text_node, length, 0, data);

        // get the current node as the next text node (whose text has been combined into the text node's text)
        node* current_node = text_node->next_sibling;
        auto* live_ranges = javascript::realms::surrounding_agent().get<ext::vector<ranges::range*>*>("live_ranges");

        // iterate by incrementing the current_node to the next sibling
        while (helpers::trees::is_exclusive_text_node(current_node))
        {
            // ranges whose starting node is current_node: increment the starting offset by the length of the text of
            // the text node (text has shifted back to previous node) and set the starting node to the text node
            live_ranges
                    ->filter([current_node](ranges::range* range) {return range->start_container == current_node;})
                    .for_each([text_node, length](ranges::range* range) {range->start_offset += length; range->start_container = text_node;});

            // ranges whose ending node is current_node: increment the ending offset by the length of the text of this
            // text node (text has shifted back to previous node) abd set the ending node to the text node
            live_ranges
                    ->filter([current_node](ranges::range* range) {return range->end_container == current_node;})
                    .for_each([text_node, length](ranges::range* range) {range->end_offset += length; range->end_container = text_node;});

            // ranges whose starting node is current_node's parent: set the starting offset to the length of the text in
            // the text node and set the starting node to the text node TODO : why?
            live_ranges
                    ->filter([current_node](ranges::range* range) {return range->start_container == current_node->parent;})
                    .for_each([length, text_node](ranges::range* range) {range->start_offset = length; range->start_container = text_node;});

            // ranges whose ending node is current_node's parent: set the ending offset to the length of the text in
            // the text node and set the ending node to the text node TODO : why?
            live_ranges
                    ->filter([current_node](ranges::range* range) {return range->end_container == current_node->parent;})
                    .for_each([length, text_node](ranges::range* range) {range->end_offset = length; range->end_container = text_node;});

            // increment the length by the current_node's length (so that the next current_node's offset can be
            // incremented further as needed to be, and set the current node to the next sibling
            length += helpers::trees::length(current_node);
            current_node = current_node->next_sibling;
        }
    }

    // remove all the empty contiguous text node's that are now empty
    for (text* text_node: helpers::trees::contiguous_text_nodes(this))
        helpers::mutation_algorithms::remove(text_node);
}


auto dom::nodes::node::has_child_nodes() const
        -> bool
{
    // return if there are any children
    return not child_nodes->empty();
}

auto dom::nodes::node::contains(node* other) const
        -> bool
{
    // return if the other node is a descendant of this node
    return helpers::trees::is_descendant(other, this);
}

auto dom::nodes::node::is_equal_node(node* other) const
        -> bool
{
    // base implementation (derived nodes will extend this method)

    // if the other node is null, then the nodes aren't equal
    if (not other)
        return false;

    // if the length of the child nodes are different, then the nodes aren't equal
    if (child_nodes->length() != other->child_nodes->length())
        return false;

    // check that the children are also equal (recursive algorithm)
    for (size_t child_index = 0; child_index < child_nodes->length(); ++child_index)
    {
        if (not child_nodes->at(child_index)->is_equal_node(other->child_nodes->at(child_index))) return false;
    }

    // return that all the base checks pass
    return true;
}

auto dom::nodes::node::is_default_namespace(
        const ext::string& namespace_) const
        -> bool
{
    // return if the namespace of this node equals namespace generated from locating the empty namespace in this node
    return namespace_ == helpers::node_internals::locate_a_namespace(this, "");
}


auto dom::nodes::node::lookup_prefix(
        const ext::string& namespace_) const
        -> ext::string
{
    // element node: return the lookup for the element
    if (auto* element_node = dynamic_cast<const element*>(this))
        return helpers::node_internals::locate_a_namespace_prefix(element_node, namespace_);

    // document node: return the lookup for the document element
    if (auto* document_node = dynamic_cast<const document*>(this))
        return helpers::node_internals::locate_a_namespace_prefix(document_node->document_element, namespace_);

    // document type node: return nullptr
    if (auto* document_type_node = dynamic_cast<const document_type*>(this))
        return nullptr;

    // document fragment: return nullptr
    if (auto* document_fragment_node = dynamic_cast<const document_fragment*>(this))
        return nullptr;

    // attribute node: return the lookup for the owner element
    if (auto* attribute_node = dynamic_cast<const attr*>(this))
        return helpers::node_internals::locate_a_namespace_prefix(attribute_node->owner_element, namespace_);

    // default: return the lookup for the parent element if it exists, otherwise nullptr
    return parent_element
            ? helpers::node_internals::locate_a_namespace_prefix(parent_element, namespace_)
            : nullptr;
}


auto dom::nodes::node::lookup_namespace_uri(
        const ext::string& prefix) const
        -> ext::string
{
    // lookup the namespace with the prefix
    return helpers::node_internals::locate_a_namespace(this, prefix);
}


auto dom::nodes::node::compare_document_position(
        node* other) const
        -> ushort
{
    // if the nodes are the same then return 0 ie there is no comparison to be done
    if (this == other) return 0;

    // set the nodes to the other node and this node, and the attributes to nullptr (don't exist yet)
    node* node_1 = other;
    node* node_2 = const_cast<node*>(this);
    attr* attr_1 = nullptr;
    attr* attr_2 = nullptr;

    // set helper variables for element representations of the node variables
    element* node_1_as_element = nullptr;
    element* node_2_as_element = nullptr;

    // if other is an attr: set attr_1 to the attribute and node_1 to the owner element
    if (dynamic_cast<const attr*>(node_1))
    {
        const node* temp = node_1;
        node_1_as_element = attr_1->owner_element;
        attr_1 = (attr*)dynamic_cast<const attr*>(temp);
    }

    // if this is an attr: set attr_2 to the attribute and node_1 to the owner element
    if (dynamic_cast<const attr*>(node_2))
    {
        const node* temp = node_2;
        node_2_as_element = attr_2->owner_element;
        attr_2 = (attr*)dynamic_cast<const attr*>(temp);
    }

    // if both nodes are attributes with the same owner element
    if (attr_1 and attr_2 and attr_1 == attr_2 and node_1_as_element == node_2_as_element)
    {
        // iterate over this node's attributes for attribute position comparison
        for (attr* attribute: *node_2_as_element->attributes)
        {
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
            : DOCUMENT_POSITION_FOLLOWING;
}


auto dom::nodes::node::get_root_node(
        const ext::string_any_map& options) const
        -> node*
{
    // return the shadow root if the <composed> option is true, otherwise the exposed tree root
    return options.at("composed").to<bool>()
            ? helpers::shadows::shadow_including_root(this)
            : helpers::trees::root(this);
}


auto dom::nodes::node::clone_node(
        bool deep) const
        -> node*
{
    // if the root of the tree of this node is a shadow root, then throw a not support error
    helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "cannot clone a shadow root node",
            [this] {return helpers::shadows::is_shadow_root(this);});

    // return the clone helper method
    return helpers::node_internals::clone(this, nullptr, deep);
}


auto dom::nodes::node::insert_before(
        node* new_node,
        node* child)
        -> node*
{
    // pre-insert the new_node before the child
    helpers::mutation_algorithms::pre_insert(new_node, this, child);

    // custom element reactions
    handle_ce_reactions(&node::insert_before);
}


auto dom::nodes::node::append_child(
        node* new_node)
        -> node*
{
    // append the new_node
    helpers::mutation_algorithms::append(new_node, this);

    // custom element reactions
    handle_ce_reactions(&node::append_child);
}


auto dom::nodes::node::replace_child(
        node* old_node,
        node* new_node)
        -> node*
{
    // replace the old_node with the new_node
    helpers::mutation_algorithms::replace(new_node, old_node, this);

    // custom element reactions
    handle_ce_reactions(&node::replace_child);
}


auto dom::nodes::node::remove_child(
        node* old_node)
        -> node*
{
    // remove the old node
    helpers::mutation_algorithms::pre_remove(old_node, this);

    // custom element reactions
    handle_ce_reactions(&node::remove_child);
}


auto dom::nodes::node::get_node_value() const
        -> ext::string
{
    // return the default node value - empty (abstract class)
    return "";
}


auto dom::nodes::node::get_text_content() const
        -> ext::string
{
    // return the default text content - empty (abstract class)
    return "";
}


auto dom::nodes::node::get_is_connected() const
        -> bool
{
    // return if this node is connected, by calling the is_connected helper method
    return helpers::shadows::is_connected(this);
}


auto dom::nodes::node::get_base_uri() const
        -> ext::string
{
    // return the serialization if the document's base uri
    return url::helpers::serializing::serialize_url(owner_document->base_uri);
}


auto dom::nodes::node::get_first_child() const
        -> node*
{
    // return the first child node
    return child_nodes->front();
}


auto dom::nodes::node::get_last_child() const
        -> node*
{
    // return the last child node
    return child_nodes->back();
}


auto dom::nodes::node::get_previous_sibling() const
        -> node*
{
    // return the previous sibling of this node by getting the item before this node in the parent's child list
    return parent->child_nodes->item_before(this);
}


auto dom::nodes::node::get_next_sibling() const
        -> node*
{
    // return the next sibling of this node by getting the item after this node in the parent's child list
    return parent->child_nodes->item_after(this);
}


auto dom::nodes::node::get_parent_element() const
        -> element*
{
    // get the parent node of this noe if it is element, otherwise null (dynamic_cast handles both cases)
    return ext::property_dynamic_cast<element*>(parent);
}


auto dom::nodes::node::set_parent_node(node* val)
        -> void
{
    // remove this node from the current parent's child node list TODO : move to mutation algorithms?
    if (parent and parent_element->child_nodes->contains(this))
        parent->child_nodes->remove(this);

    // set the new parent and append this to the child nodes TODO : move to mutation algorithms
    parent << val;
    parent->child_nodes->append(this);

    // if this node can be rendered
    if (m_rendered_widget->isWidgetType())
    {
        // scrollable widget parent: append to the QScrollArea's internal widget's layout
        if (auto* new_parent_scroll_widget = dynamic_cast<QScrollArea*>(val->qt()))
        {
            auto* this_widget = dynamic_cast<QWidget*>(m_rendered_widget);
            new_parent_scroll_widget->widget()->layout()->addWidget(this_widget);
        }

        // widget action (for menus and similar) parent: append to the TODO
        else if (auto new_parent_action_widget = dynamic_cast<QWidgetAction*>(val->qt()))
        {
            auto* this_widget = dynamic_cast<QWidget*>(m_rendered_widget);
            new_parent_action_widget->setDefaultWidget(this_widget);
        }

        // widget parent: append to the QWidget's layout
        else if (auto* new_parent_widget = dynamic_cast<QWidget*>(val->qt()))
        {
            auto* this_widget = dynamic_cast<QWidget*>(m_rendered_widget);
            new_parent_widget->layout()->addWidget(this_widget);
        }

        // should never reach here (failsafe for incorrect c++ code implementations)
        else {
            std::cout << "error widget type trying to qt" << std::endl;
            delete m_rendered_widget;
            return;
        }

        // show the widget render of this widget (sometimes changing the parent can hide this widget)
        m_rendered_widget->show();
    }
}


auto dom::nodes::node::qt() const
        -> QWidget*
{
    return nullptr;
}


auto dom::nodes::node::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<node>{isolate}
            .inherit<event_target>()
            .template static_("DOCUMENT_POSITION_DISCONNECTED", node::DOCUMENT_POSITION_DISCONNECTED, true)
            .template static_("DOCUMENT_POSITION_PRECEDING", node::DOCUMENT_POSITION_PRECEDING, true)
            .template static_("DOCUMENT_POSITION_FOLLOWING", node::DOCUMENT_POSITION_FOLLOWING, true)
            .template static_("DOCUMENT_POSITION_CONTAINS", node::DOCUMENT_POSITION_CONTAINS, true)
            .template static_("DOCUMENT_POSITION_CONTAINED_BY", node::DOCUMENT_POSITION_CONTAINED_BY, true)
            .template static_("DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC", node::DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC, true)
            .function("normalize", &node::normalize)
            .function("hasChildNodes", &node::has_child_nodes)
            .function("contains", &node::contains)
            .function("isEqualNode", &node::is_equal_node)
            .function("isDefaultNamespace", &node::is_default_namespace)
            .function("lookupPrefix", &node::lookup_prefix)
            .function("lookupNamespaceURI", &node::lookup_namespace_uri)
            .function("compareDocumentPosition", &node::compare_document_position)
            .function("getRootNode", &node::get_root_node)
            .function("cloneNode", &node::clone_node)
            .function("insertBefore", &node::insert_before)
            .function("appendChild", &node::append_child)
            .function("replaceChild", &node::replace_child)
            .function("removeChild", &node::remove_child)
            .var("nodeName", &node::node_name)
            .var("nodeValue", &node::node_value)
            .var("textContent", &node::text_content)
            .var("baseURI", &node::base_uri)
            .var("isConnected", &node::is_connected)
            .var("childNodes", &node::child_nodes)
            .var("parentNode", &node::parent)
            .var("parentElement", &node::parent_element)
            .var("ownerDocument", &node::owner_document)
            .var("firstChild", &node::first_child)
            .var("lastChild", &node::last_child)
            .var("previousSibling", &node::previous_sibling)
            .var("nextSibling", &node::next_sibling)
            .auto_wrap_objects();
}
