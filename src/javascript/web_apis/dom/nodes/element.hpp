#pragma once
#ifndef SBROWSER_ELEMENT_HPP
#define SBROWSER_ELEMENT_HPP

#include <queue>

#include <dom/mixins/child_node.hpp>
#include <dom/mixins/document_or_element_node.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>
#include <dom/mixins/parent_node.hpp>
#include <dom/mixins/slottable.hpp>
#include <dom/nodes/node.hpp>

#include <veque.hpp>

namespace dom
{
    namespace helpers
    {
        struct attributes;
        struct custom_elements;
        struct node_internals;
    }
    namespace internal {struct custom_element_definition;}
    namespace nodes
    {
        class attr;
        class element;
        class shadow_root;
    }
}

namespace geometry::shapes{class dom_rect;}


/*
 * https://dom.spec.whatwg.org/#interface-element
 * https://developer.mozilla.org/en-US/docs/Web/API/Element
 *
 * Element is the most general base class from which all element objects (i.e. objects that represent elements) in a
 * Document inherit. It only has methods and properties common to all kinds of elements. More specific classes inherit
 * from Element.
 *
 * For example, the HTMLElement interface is the base interface for HTML elements, while the SVGElement interface is the
 * basis for all SVG elements. Most functionality is specified further down the class hierarchy.
 *
 * Languages outside the realm of the Web platform, like XUL through the XULElement interface, also implement Element.
 */
class dom::nodes::element
        : public node
        , public mixins::parent_node<element>
        , public mixins::non_document_type_child_node<element>
        , public mixins::child_node<element>
        , public mixins::slottable<element>
        , public mixins::document_or_element_node<element>
        // , public css::cssom_view::mixins::scrollable<element>
        // , public css::cssom_view::mixins::geometry_utils<element> {
{
public: friends
    friend class document;
    friend struct helpers::attributes;
    friend struct helpers::custom_elements;
    friend struct helpers::node_internals;

public: constructors
    element();
    ~element() override;

public: methods
    // dom
    auto has_attributes() const -> bool;
    auto get_attribute_names() const -> ext::vector<ext::string>;

    auto has_attribute(const ext::string& name) const -> bool;
    auto has_attribute_ns(const ext::string& namespace_, const ext::string& local_name) const -> bool;
    auto has_attribute_node(attr* attribute) const -> bool;
    auto has_attribute_node_ns(attr* attribute) const -> bool;

    auto get_attribute(const ext::string& qualified_name) const -> ext::string;
    auto get_attribute_ns(const ext::string& namespace_, const ext::string& local_name) const -> ext::string;
    auto get_attribute_node(const ext::string& qualified_name) const -> attr*;
    auto get_attribute_node_ns(const ext::string& namespace_, const ext::string& local_name) const -> attr*;

    auto set_attribute(const ext::string& qualified_name, const ext::string& value) -> void;
    auto set_attribute_ns(const ext::string& namespace_, const ext::string& qualified_name, const ext::string& value) -> void;
    auto set_attribute_node(attr* attribute) -> attr*;
    auto set_attribute_node_ns(attr* attribute) -> attr*;

    auto remove_attribute(const ext::string& qualified_name) -> void;
    auto remove_attribute_ns(const ext::string& namespace_, const ext::string& local_name) -> void;
    auto remove_attribute_node(attr* attribute) -> attr*;
    auto remove_attribute_node_ns(attr* attribute) -> attr*;

    auto toggle_attribute(const ext::string& qualified_name, bool force = false) -> bool;
    auto toggle_attribute_ns(const ext::string& namespace_, const ext::string& local_name, bool force = false) -> bool;
    auto toggle_attribute_node(attr* attribute, bool force = false) -> attr*;
    auto toggle_attribute_node_ns(attr* attribute, bool force = false) -> attr*;

    new_obj auto attach_shadow(const ext::string_any_map& options) -> shadow_root;
    same_obj auto closest(const ext::string& selectors) -> element*;
    same_obj auto matches(const ext::string& selectors) -> bool;

    // css-spatial-navigation
    auto get_spatial_navigation_container() -> node*;
    auto spatial_navigation_search(const ext::string& direction, const ext::string_any_map& options = {}) -> node*;
    auto focusable_areas(const ext::string_any_map& options = {}) -> ext::vector<node*>;

    // css-pseudo
    // css::css_pseudo::elements::pseudo_element* pseudo(const ext::string& type); TODO

    // cssom-view
    auto get_client_rects() const -> ext::vector<geometry::shapes::dom_rect>;
    auto get_bounding_client_rect() const -> geometry::shapes::dom_rect;
    auto scroll_into_view(const ext::string_any_map& options = {}) -> void;

public: properties
    // dom
    ext::dom_property<ext::string, _F> namespace_uri;
    ext::dom_property<ext::string, _F> prefix;
    ext::dom_property<ext::string, _F> local_name;
    ext::dom_property<ext::string, _F> tag_name;
    ext::dom_property<ext::string, _T> class_name;
    ext::dom_property<ext::string, _T> slot;
    ext::dom_property<ext::string, _T> id;
    ext::dom_property<shadow_root*, _F> shadow_root_node;
    ext::dom_property<ext::vector<attr*>*, _F> attributes;
    ext::dom_property<ext::vector<ext::string>*, _F> class_list;

    // cssom-view
    ext::dom_property<double, _F> scroll_top;
    ext::dom_property<double, _F> scroll_left;
    ext::dom_property<double, _F> scroll_width;
    ext::dom_property<double, _F> scroll_height;
    ext::dom_property<double, _F> client_top;
    ext::dom_property<double, _F> client_left;
    ext::dom_property<double, _F> client_width;
    ext::dom_property<double, _F> client_height;

    // cs-shadow-parts
    ext::dom_property<ext::vector<ext::string>*, _F> parts;

public: internal_methods
    auto render() const -> QWidget* override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private: internal_properties
    ext::property<ext::string> m_qualified_name;
    ext::property<ext::string> m_html_uppercase_qualified_name;

    ext::string m_is;
    ext::string m_custom_element_state;
    internal::custom_element_definition* m_custom_element_definition = nullptr;
    std::queue<std::function<void(element*)>> m_custom_element_reaction_queue;

private: accessors
    // dom
    auto get_text_content() const -> ext::string override;
    auto get_tag_name() const -> ext::string;
    auto get_shadow_root() const -> shadow_root*;

    auto set_text_content(const ext::string& val) -> void override;

    // cssom-view
    auto get_scroll_top() const -> double;
    auto get_scroll_left() const -> double;
    auto get_scroll_width() const -> double;
    auto get_scroll_height() const -> double;
    auto get_client_top() const -> double;
    auto get_client_left() const -> double;
    auto get_client_width() const -> double;
    auto get_client_height() const -> double;

    auto set_scroll_top(double val) -> void;
    auto set_scroll_left(double val) -> void;

    auto get_m_qualified_name() const -> ext::string;
    auto get_m_html_qualified_uppercase_name() const -> ext::string;

private: internal_properties
    ext::vector<ext::string> m_shadow_attachable_local_names = {
            "article", "aside", "blockquote", "body", "div", "footer", "h1", "h2", "h3", "h4", "h5", "h6", "header",
            "main", "nav", "p", "section", "span"
    };
};


#endif //SBROWSER_ELEMENT_HPP
