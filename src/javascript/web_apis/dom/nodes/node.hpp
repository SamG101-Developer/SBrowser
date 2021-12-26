#pragma once
#ifndef SBROWSER_NODE_HPP
#define SBROWSER_NODE_HPP

#include <ext/property.hpp>
#include <ext/map.hpp>
#include <ext/string.hpp>

#include <dom/nodes/event_target.hpp>

#include <QtWidgets/QWidget>

namespace dom {
    namespace nodes {
        class document;
        class element;
        class node;
    }

    namespace helpers {struct mutation_observers;}
    namespace internal {struct registered_observer;}
    namespace mutations {class mutation_observer;}
}


class dom::nodes::node : public event_target {

friends
    friend struct helpers::mutation_observers;
    friend class mutations::mutation_observer;

public constructors:
    node();
    ~node() override;

public static_constants:
    static const unsigned short DOCUMENT_POSITION_DISCONNECTED = 0x01;
    static const unsigned short DOCUMENT_POSITION_PRECEDING = 0x02;
    static const unsigned short DOCUMENT_POSITION_FOLLOWING = 0x04;
    static const unsigned short DOCUMENT_POSITION_CONTAINS = 0x08;
    static const unsigned short DOCUMENT_POSITION_CONTAINED_BY = 0x10;
    static const unsigned short DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = 0x20;

    static const unsigned short ELEMENT_NODE = 1;
    static const unsigned short ATTRIBUTE_NODE = 2;
    static const unsigned short TEXT_NODE = 3;
    static const unsigned short CDATA_SECTION_NODE = 4;
    static const unsigned short PROCESSING_INSTRUCTION_NODE = 7;
    static const unsigned short COMMENT_NODE = 8;
    static const unsigned short DOCUMENT_NODE = 9;
    static const unsigned short DOCUMENT_TYPE_NODE = 10;
    static const unsigned short DOCUMENT_FRAGMENT_NODE = 11;

public methods:
    void normalize() const;
    bool has_child_nodes() const;
    bool contains(node* other) const;
    bool is_equal_node(node* other) const;
    bool is_default_namespace(ext::cstring& namespace_) const;
    ext::string lookup_prefix(ext::cstring& namespace_) const;
    ext::string lookup_namespace_uri(ext::cstring& prefix) const;
    unsigned short compare_document_position(node* other) const;
    dom::nodes::node* get_root_node(ext::cstring_any_map& options) const;
    dom::nodes::node* clone_node(bool deep = false) const;
    dom::nodes::node* insert_before(node* new_node, node* child);
    dom::nodes::node* append_child(node* new_node);
    dom::nodes::node* replace_child(node* old_node, node* new_node);
    dom::nodes::node* remove_child(node* old_node);

public properties:
    ext::dom_property<unsigned short> node_type;
    ext::dom_property<ext::string> node_name{true};
    ext::dom_property<ext::string> node_value{true};
    ext::dom_property<ext::string> text_content{true};
    ext::dom_property<ext::string> base_uri;
    ext::dom_property<bool> is_connected;
    ext::dom_property<ext::vector<node*>*> child_nodes;
    ext::dom_property<node*> parent_node;
    ext::dom_property<element*> parent_element;
    ext::dom_property<document*> owner_document;
    ext::dom_property<node*> first_child;
    ext::dom_property<node*> last_child;
    ext::dom_property<node*> previous_sibling;
    ext::dom_property<node*> next_sibling;

protected accessors:
    inline virtual ext::string get_node_value() const;
    inline virtual ext::string get_text_content() const;

    inline virtual void set_node_value(ext::cstring& val);
    inline virtual void set_text_content(ext::cstring& val);

protected internal_methods:
    virtual bool equals(nodes::node* other);
    virtual QWidget* render() {return nullptr;};

protected internal_properties:
    QWidget* m_rendered_widget;

private accessors:
    inline bool get_is_connected() const;
    inline ext::string get_base_uri() const;
    inline node* get_first_child() const;
    inline node* get_last_child() const;
    inline node* get_previous_sibling() const;
    inline node* get_next_sibling() const;
    inline element* get_parent_element() const;

    inline void set_parent_node(node* val);

    ext::vector<internal::registered_observer*>* m_registered_observer_list;
};


#endif //SBROWSER_NODE_HPP
