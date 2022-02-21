#pragma once
#ifndef SBROWSER_NODE_INTERNALS_HPP
#define SBROWSER_NODE_INTERNALS_HPP

#include <ext/iterables/string.hpp>

namespace dom {
    namespace helpers {struct node_internals;}
    namespace nodes {
        class document;
        class element;
        class node;
        class window;
        class window_proxy;
    }
}

namespace html::elements {class html_element;}


struct dom::helpers::node_internals {
    template <typename T> static auto clone(T* node, nodes::document* document = nullptr, bool deep = false) -> dom::nodes::node* requires std::is_base_of_v<T, nodes::node>;
    static auto locate_a_namespace_prefix(const nodes::element* element, ext::cstring& namespace_) -> ext::string;
    static auto locate_a_namespace(const nodes::node* node, ext::cstring& prefix) -> ext::string;
    static auto list_of_elements_with_qualified_name(nodes::node* descendant_element, ext::cstring& qualified_name) -> ext::vector<nodes::element*>;
    static auto list_of_elements_with_namespace_and_local_name(nodes::node* node, ext::cstring& namespace_, ext::cstring& local_name) -> ext::vector<nodes::element*>;
    static auto list_of_elements_with_class_names(nodes::node* node, ext::cstring& class_names) -> ext::vector<nodes::element*>;
    static auto adopt(nodes::node* node, nodes::document* document) -> void;

    static auto string_replace_all(ext::cstring& string, nodes::node* parent) -> void;
    static auto is_document_available_to_user(nodes::document* document, ext::cstring&) -> bool;  // TODO name [-1]
    static auto is_document_fully_active(nodes::document* document) -> bool;
    static auto is_html(const nodes::element* element) -> bool;

    static auto has_active_parser(nodes::document* document) -> bool;
    static auto advisory_information(html::elements::html_element* element) -> ext::string;

    static auto document_has_style_sheets_blocking_scripts(nodes::document* document) -> bool;

    template <typename ...nodes_or_strings> static auto convert_nodes_into_node(nodes::document* document, nodes_or_strings... nodes) -> nodes::node*;
};


#endif //SBROWSER_NODE_INTERNALS_HPP
