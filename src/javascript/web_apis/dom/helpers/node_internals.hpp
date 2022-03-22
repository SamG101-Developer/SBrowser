#pragma once
#ifndef SBROWSER_NODE_INTERNALS_HPP
#define SBROWSER_NODE_INTERNALS_HPP

#include <ext/string.hpp>

namespace dom::helpers {struct node_internals;}
namespace dom::nodes {class document;}
namespace dom::nodes {class element;}
namespace dom::nodes {class node;}
namespace dom::nodes {class window;}
namespace dom::nodes {class window_proxy;}
namespace html::elements {class html_element;}


struct dom::helpers::node_internals final
{
    // node manipulation
    template <typename T>
    static auto clone(
            T* node,
            nodes::document* document = nullptr,
            bool deep = false)
            -> dom::nodes::node*
            requires std::is_base_of_v<T, nodes::node>;

    static auto adopt(
            nodes::node* node,
            const nodes::document* document)
            -> void;

    // locating information on a node
    static auto locate_a_namespace_prefix(
            const nodes::element* element,
            const ext::string& namespace_)
            -> ext::string;

    static auto locate_a_namespace(
            const nodes::node* node,
            const ext::string& prefix)
            -> ext::string;

    // list elements with certain features
    static auto list_of_elements_with_qualified_name(
            const nodes::node* descendant_element,
            const ext::string& qualified_name)
            -> ext::vector<nodes::element*>;

    static auto list_of_elements_with_namespace_and_local_name(
            const nodes::node* node,
            const ext::string& namespace_,
            const ext::string& local_name)
            -> ext::vector<nodes::element*>;

    static auto list_of_elements_with_class_names(
            const nodes::node* node,
            const ext::string& class_names)
            -> ext::vector<nodes::element*>;

    // TODO : arrange
    static auto string_replace_all(const ext::string& string, nodes::node* parent) -> void;
    static auto is_document_available_to_user(nodes::document* document, const ext::string&) -> bool;  // TODO name [-1]
    static auto is_document_fully_active(nodes::document* document) -> bool;
    static auto is_html(const nodes::element* element) -> bool;

    static auto has_active_parser(nodes::document* document) -> bool;
    static auto advisory_information(html::elements::html_element* element) -> ext::string;

    static auto document_has_style_sheets_blocking_scripts(nodes::document* document) -> bool;

    template <typename ...nodes_or_strings_t> static auto convert_nodes_into_node(const nodes::document* document, nodes_or_strings_t... nodes) -> nodes::node*;
};


#endif //SBROWSER_NODE_INTERNALS_HPP
