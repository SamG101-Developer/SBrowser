#pragma once
#ifndef SBROWSER_ATTR_HPP
#define SBROWSER_ATTR_HPP

#include <dom/nodes/node.hpp>

namespace dom::nodes {
    class attr;
    class element;
}


class dom::nodes::attr : public node {
public constructors:
    attr();

public properties:
    ext::dom_property<ext::string, _F> namespace_uri;
    ext::dom_property<ext::string, _F> prefix;
    ext::dom_property<ext::string, _F> local_name;
    ext::dom_property<ext::string, _F> name;
    ext::dom_property<ext::string, _F> value;
    ext::dom_property<element*, _F> owner_element;

protected internal_properties:
    ext::property<ext::string> m_qualified_name;

private accessors:
    ext::string get_node_value() const override;
    ext::string get_text_content() const override;
    ext::string get_m_qualified_name() const;

    void set_node_value(ext::cstring& val) override;
    void set_text_content(ext::cstring& val) override;
    void set_value(ext::cstring& val);
};


#endif //SBROWSER_ATTR_HPP
