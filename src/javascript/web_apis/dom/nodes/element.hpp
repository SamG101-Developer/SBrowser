#pragma once
#ifndef SBROWSER_ELEMENT_HPP
#define SBROWSER_ELEMENT_HPP

#include <queue>
#include <dom/nodes/node.hpp>

#include <dom/mixins/child_node.hpp>
#include <dom/mixins/document_or_element_node.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>
#include <dom/mixins/parent_node.hpp>
#include <dom/mixins/slottable.hpp>

namespace dom {
    namespace helpers {
        struct attributes;
        struct custom_elements;
        struct node_internals;
    }

    namespace internal {struct custom_element_definition;}

    namespace nodes {
        class attr;
        class element;
        class shadow_root;
    }
}

namespace geometry::shapes{class dom_rect;}


class dom::nodes::element
        : public node
        , public mixins::parent_node<element>
        , public mixins::non_document_type_child_node<element>
        , public mixins::child_node<element>
        , public mixins::slottable<element>
        , public mixins::document_or_element_node<element> {
//        , public css::cssom_view::mixins::scrollable<element>
//        , public css::cssom_view::mixins::geometry_utils<element> {

friends
    friend struct helpers::attributes;
    friend struct helpers::custom_elements;
    friend struct helpers::node_internals;

public constructors:
    element();
    ~element() override;

public methods:
    // dom
    bool has_attributes() const;
    bool has_attribute(ext::cstring& name) const;
    bool has_attribute_ns(ext::cstring& namespace_, ext::cstring& local_name) const;
    ext::vector<ext::string> get_attribute_names() const;

    ext::string get_attribute(ext::cstring& qualified_name) const;
    ext::string get_attribute_ns(ext::cstring& namespace_, ext::string local_name) const;
    attr* get_attribute_node(ext::cstring& qualified_name) const;
    attr* get_attribute_node_ns(ext::cstring& namespace_, ext::cstring& local_name) const;

    void set_attribute(ext::cstring& qualified_name, ext::cstring& value);
    void set_attribute_ns(ext::cstring& namespace_, ext::cstring& qualified_name, ext::cstring& value);
    attr* set_attribute_node(attr* attribute);
    attr* set_attribute_node_ns(attr* attribute);

    void remove_attribute(ext::cstring& qualified_name);
    void remove_attribute_ns(ext::cstring& namespace_, ext::cstring& local_name);
    attr* remove_attribute_node(attr* attribute);
    attr* remove_attribute_node_ns(attr* attribute);

    bool toggle_attribute(ext::cstring& qualified_name, bool force = false);
    bool toggle_attribute_ns(ext::cstring& namespace_, ext::cstring& local_name, bool force = false);
    attr* toggle_attribute_node(attr* attribute, bool force = false);
    attr* toggle_attribute_node_ns(attr* attribute, bool force = false);

    shadow_root* attach_shadow(ext::cstring_any_map& options);
    element* closest(ext::cstring& selectors);
    bool matches(ext::cstring& selectors);

    // css-spatial-navigation
    node* get_spatial_navigation_container();
    node* spatial_navigation_search(ext::cstring& direction, ext::cstring_any_map& options = {});
    ext::vector<node*> focusable_areas(ext::cstring_any_map& options = {});

    // css-pseudo
//    css::css_pseudo::elements::pseudo_element* pseudo(ext::cstring& type); TODO

    // cssom-view
    ext::vector<geometry::shapes::dom_rect> get_client_rects();
    geometry::shapes::dom_rect get_bounding_client_rect();
    void scroll_into_view(ext::cstring_any_map& options = {});

public properties:
    // dom
    ext::dom_property<ext::string> namespace_uri;
    ext::dom_property<ext::string> prefix;
    ext::dom_property<ext::string> local_name;
    ext::dom_property<ext::string> tag_name;
    ext::dom_property<ext::string> class_name{true};
    ext::dom_property<ext::string> slot{true};
    ext::dom_property<ext::string> id{true};
    ext::dom_property<shadow_root*> shadow_root_node;
    ext::dom_property<ext::vector<attr*>*> attributes;
    ext::dom_property<ext::vector<ext::string>> class_list;

    // cssom-view
    ext::dom_property<double> scroll_top;
    ext::dom_property<double> scroll_left;
    ext::dom_property<double> scroll_width;
    ext::dom_property<double> scroll_height;
    ext::dom_property<double> client_top;
    ext::dom_property<double> client_left;
    ext::dom_property<double> client_width;
    ext::dom_property<double> client_height;

    // cs-shadow-parts
    ext::dom_property<ext::vector<ext::string>*> parts;

protected internal_properties:
    ext::property<ext::string> m_qualified_name;
    ext::property<ext::string> m_html_uppercase_qualified_name;

    ext::string m_is;
    ext::string m_custom_element_state;
    internal::custom_element_definition* m_custom_element_definition;
    std::queue<std::function<void(element*)>> m_custom_element_reaction_queue;

public internal_methods:
    QWidget* render() override;

private accessors:
    // dom
    inline ext::string get_text_content() const override;
    inline ext::string get_tag_name() const;
    inline shadow_root* get_shadow_root() const;

    inline void set_text_content(ext::string val);
    inline void set_id(ext::string val);

    // cssom-view
    inline double get_scroll_top() const;
    inline double get_scroll_left() const;
    inline double get_scroll_width() const;
    inline double get_scroll_height() const;
    inline double get_client_top() const;
    inline double get_client_left() const;
    inline double get_client_width() const;
    inline double get_client_height() const;

    inline void set_scroll_top(double val);
    inline void set_scroll_left(double val);

    inline ext::string get_m_qualified_name() const;
    inline ext::string get_m_html_qualified_uppercase_name() const;

private internal_properties:
    ext::vector<ext::string> m_local_names = {
            "article", "aside", "blockquote", "body", "div", "footer", "h1", "h2", "h3", "h4", "h5", "h6", "header",
            "main", "nav", "p", "section", "span"
    };
};


#endif //SBROWSER_ELEMENT_HPP
