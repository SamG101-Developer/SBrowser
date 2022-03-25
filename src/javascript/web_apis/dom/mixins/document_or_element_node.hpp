#pragma once
#ifndef SBROWSER_DOCUMENT_OR_ELEMENT_NODE_HPP
#define SBROWSER_DOCUMENT_OR_ELEMENT_NODE_HPP

#include <ext/vector.hpp>
#include <ext/string.hpp>
#include <dom_object.hpp>

namespace dom::mixins {template <typename T> class document_or_element_node;}
namespace dom::nodes {class element;}


template <typename T>
class dom::mixins::document_or_element_node : virtual public dom_object
{
public methods:
    auto get_elements_by_tag_name(const ext::string& qualified_name) -> ext::vector<nodes::element*>;
    auto get_elements_by_tag_name_ns(const ext::string& namespace_, const ext::string& local_name) -> ext::vector<nodes::element*>;
    auto get_elements_by_class_name(const ext::string& class_names) -> ext::vector<nodes::element*>;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_DOCUMENT_OR_ELEMENT_NODE_HPP
