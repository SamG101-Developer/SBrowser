#pragma once
#ifndef SBROWSER_ATTR_HPP
#define SBROWSER_ATTR_HPP

#include <dom/nodes/node.hpp>

namespace dom::nodes
{
    class attr;
    class element;
}


/*
 * https://dom.spec.whatwg.org/#interface-attr
 * https://developer.mozilla.org/en-US/docs/Web/API/Attr
 *
 * The Attr interface represents one of an element's attributes as an object. In most situations, you will directly
 * retrieve the attribute value as a string (e.g., Element.getAttribute()), but certain functions (e.g.,
 * Element.getAttributeNode()) or means of iterating return Attr instances.
 *
 * The core idea of an object of type Attr is the association between a name and a value. An attribute may also be part
 * of a namespace and, in this case, it also has a URI identifying the namespace, and a prefix that is an abbreviation
 * for the namespace.
 *
 * The name is deemed local when it ignores the eventual namespace prefix and deemed qualified when it includes the
 * prefix of the namespace, if any, separated from the local name by a colon (:). We have three cases: an attribute
 * outside of a namespace, an attribute inside a namespace without a prefix defined, an attribute inside a namespace
 * with a prefix:
 *
 * Attribute	Namespace name	Namespace prefix	Attribute local name	Attribute qualified name
 * myAttr	    none	        none	            myAttr	                myAttr
 * myAttr	    mynamespace	    none	            myAttr	                myAttr
 * myAttr	    mynamespace	    myns	            myAttr	                myns:myAttr
 */
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
    auto v8(v8::Isolate* isolate) const -> ext::any override;

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
