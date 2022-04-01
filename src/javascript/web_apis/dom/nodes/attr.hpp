#pragma once
#ifndef SBROWSER_ATTR_HPP
#define SBROWSER_ATTR_HPP

#include <dom/nodes/node.hpp>

namespace dom::nodes {class attr;}
namespace dom::nodes {class element;}


class dom::nodes::attr : public node
{
public constructors:
    attr();

public js_properties:
    ext::dom_property<ext::string> namespace_uri;
    ext::dom_property<ext::string> prefix;
    ext::dom_property<ext::string> local_name;
    ext::dom_property<ext::string> name;
    ext::dom_property<ext::string> value;
    ext::dom_property<element*> owner_element;

public: cpp_methods
    auto qt() const -> QWidget* override {return nullptr;}
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_properties:
    ext::property<ext::string> m_qualified_name;

private accessors:
    auto get_node_value() const -> ext::string override;
    auto get_text_content() const -> ext::string override;
    auto get_m_qualified_name() const -> ext::string;

    auto set_node_value(const ext::string& val) -> void override;
    auto set_text_content(const ext::string& val) -> void override;
    auto set_value(const ext::string& val) -> void;
};


#endif //SBROWSER_ATTR_HPP
