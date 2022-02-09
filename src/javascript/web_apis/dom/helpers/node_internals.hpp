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
    template <typename T> static dom::nodes::node* clone(T* node, nodes::document* document = nullptr, bool deep = false) requires std::is_base_of_v<T, nodes::node>;
    static ext::string locate_a_namespace_prefix(nodes::element* element, ext::cstring& namespace_);
    static ext::string locate_a_namespace(nodes::node* node, ext::cstring& prefix);
    static ext::vector<nodes::element*> list_of_elements_with_qualified_name(nodes::node* descendant_element, ext::cstring& qualified_name);
    static ext::vector<nodes::element*> list_of_elements_with_namespace_and_local_name(nodes::node* node, ext::cstring& namespace_, ext::cstring& local_name);
    static ext::vector<nodes::element*> list_of_elements_with_class_names(nodes::node* node, ext::cstring& class_names);
    static void adopt(nodes::node* node, nodes::document* document);

    static void string_replace_all(ext::cstring& string, nodes::node* parent);
    static bool is_document_available_to_user(nodes::document* document, ext::cstring&);
    static bool is_document_fully_active(nodes::document* document); // TODO name [-1]
    static bool is_html(const nodes::element* element);

    static bool has_active_parser(nodes::document* document);
    static ext::string advisory_information(html::elements::html_element* element);

    static bool document_has_style_sheets_blocking_scripts(nodes::document* document);

    template <typename ...nodes_or_strings> static nodes::node* convert_nodes_into_node(nodes::document* document, nodes_or_strings... nodes);
};


#endif //SBROWSER_NODE_INTERNALS_HPP
