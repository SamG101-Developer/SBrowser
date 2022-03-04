#pragma once
#ifndef SBROWSER_ATTR_HPP
#define SBROWSER_ATTR_HPP

#include <dom/nodes/node.hpp>

namespace dom::nodes
{
    class attr;
    class element;
}


class dom::nodes::attr : public node
{
public constructors:
    attr();

public properties:
    ext::dom_property<ext::string, _F> namespace_uri;
    ext::dom_property<ext::string, _F> prefix;
    ext::dom_property<ext::string, _F> local_name;
    ext::dom_property<ext::string, _F> name;
    ext::dom_property<ext::string, _F> value;
    ext::dom_property<element*, _F> owner_element;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected internal_properties:
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
