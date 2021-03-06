#pragma once
#ifndef SBROWSER_NODE_HPP
#define SBROWSER_NODE_HPP

#include <ext/map.hpp>

#include <dom/nodes/event_target.hpp>

#include <veque.hpp>
#include <QtCore/QPointer>
#include <QtWidgets/QWidget>

namespace dom::nodes {class document;}
namespace dom::nodes {class element;}
namespace dom::nodes {class node;}
namespace dom::helpers {struct mutation_algorithms;}
namespace dom::helpers {struct mutation_observers;}
namespace dom::internal {struct registered_observer;}
namespace dom::mutations {class mutation_observer;}


class dom::nodes::node : public event_target
{
public friends:
    friend struct helpers::mutation_algorithms;
    friend struct helpers::mutation_observers;
    friend class mutations::mutation_observer;

public constructors:
    node();
    ~node() override;

public js_static_constants:
    static const ushort DOCUMENT_POSITION_DISCONNECTED = 0x01;
    static const ushort DOCUMENT_POSITION_PRECEDING = 0x02;
    static const ushort DOCUMENT_POSITION_FOLLOWING = 0x04;
    static const ushort DOCUMENT_POSITION_CONTAINS = 0x08;
    static const ushort DOCUMENT_POSITION_CONTAINED_BY = 0x10;
    static const ushort DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = 0x20;

    static const ushort ELEMENT_NODE = 1;
    static const ushort ATTRIBUTE_NODE = 2;
    static const ushort TEXT_NODE = 3;
    static const ushort CDATA_SECTION_NODE = 4;
    static const ushort PROCESSING_INSTRUCTION_NODE = 7;
    static const ushort COMMENT_NODE = 8;
    static const ushort DOCUMENT_NODE = 9;
    static const ushort DOCUMENT_TYPE_NODE = 10;
    static const ushort DOCUMENT_FRAGMENT_NODE = 11;

public js_methods:
    auto normalize() const -> void;
    auto has_child_nodes() const -> bool;
    auto contains(node* other) const -> bool;
    auto is_equal_node(node* other) const -> bool;
    auto is_default_namespace(const ext::string& namespace_) const -> bool;
    auto lookup_prefix(const ext::string& namespace_) const -> ext::string;
    auto lookup_namespace_uri(const ext::string& prefix) const -> ext::string;
    auto compare_document_position(node* other) const -> ushort;
    auto get_root_node(const ext::string_any_map_t& options) const -> node*;
    auto clone_node(bool deep = false) const -> node*;
    auto insert_before(node* new_node, node* child) -> node*;
    auto append_child(node* new_node) -> node*;
    auto replace_child(node* old_node, node* new_node) -> node*;
    auto remove_child(node* old_node) -> node*;

public js_properties:
    ext::property<ushort> node_type;
    ext::property<ext::string, _T> node_name;
    ext::property<ext::string, _T> node_value;
    ext::property<ext::string, _T> text_content;
    ext::property<ext::string> base_uri;
    ext::property<bool> is_connected;
    ext::property<ext::vector<node*>*> child_nodes;
    ext::property<node*> parent;
    ext::property<element*> parent_element;
    ext::property<document*> owner_document;
    ext::property<node*> first_child;
    ext::property<node*> last_child;
    ext::property<node*> previous_sibling;
    ext::property<node*> next_sibling;

public cpp_methods:
    virtual auto qt() const -> QWidget* = 0 {return nullptr;};
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected accessors:
    virtual auto get_node_value() const -> ext::string&;
    virtual auto get_text_content() const -> ext::string&;

    virtual auto set_node_value(const ext::string& val) -> void;
    virtual auto set_text_content(const ext::string& val) -> void;

protected cpp_properties:
    QWidget* m_rendered_widget = nullptr;
    ext::vector<internal::registered_observer*>& m_registered_observer_list;

protected accessors:
    virtual auto set_parent_node(node* val) -> void;

private accessors:
    auto get_is_connected() const -> bool;
    auto get_base_uri() const -> ext::string&;
    auto get_first_child() const -> node*;
    auto get_last_child() const -> node*;
    auto get_previous_sibling() const -> node*;
    auto get_next_sibling() const -> node*;
    auto get_parent_element() const -> element*;
};


#endif //SBROWSER_NODE_HPP
