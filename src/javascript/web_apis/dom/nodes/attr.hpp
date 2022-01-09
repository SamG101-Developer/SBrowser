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
    ext::dom_property<ext::string> namespace_uri;
    ext::dom_property<ext::string> prefix;
    ext::dom_property<ext::string> local_name;
    ext::dom_property<ext::string> name;
    ext::dom_property<ext::string> value;
    ext::dom_property<element*> owner_element;

protected internal_properties:
    ext::property<ext::string> m_qualified_name;

private accessors:
    inline ext::string get_node_value() const override;
    inline ext::string get_text_content() const override;
    inline ext::string get_m_qualified_name() const;

    inline void set_node_value(ext::cstring& val) override;
    inline void set_text_content(ext::cstring& val) override;
    inline void set_value(ext::cstring& val);
};


#endif //SBROWSER_ATTR_HPP
