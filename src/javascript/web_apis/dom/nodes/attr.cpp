#include "attr.hpp"

#include <dom/helpers/attributes.hpp>


dom::nodes::attr::attr()
{
    // set the property's values
    node_type = ATTRIBUTE_NODE;

    // set the custom accessor methods
    bind_get(node_value, get_node_value);
    bind_get(text_content, get_text_content);
    bind_get(m_qualified_name, get_m_qualified_name);

    bind_set(value, set_value);
    bind_set(node_value, set_node_value);
    bind_set(text_content, set_text_content);
}


auto dom::nodes::attr::get_node_value() const
        -> ext::string&
{
    // the node_value is the attribute's value
    return value;
}


auto dom::nodes::attr::get_text_content() const
        -> ext::string&
{
    // the text_content is the attribute's value
    return value;
}


auto dom::nodes::attr::get_m_qualified_name() const
        -> ext::string
{
    // the qualified name is the prefix and local name joined with a colon
    return prefix + ext::string{":"} + local_name;
}


auto dom::nodes::attr::set_node_value(
        const ext::string& val)
        -> void
{
    // the node_value is the attribute's value
    set_value(val);
}


auto dom::nodes::attr::set_text_content(
        const ext::string& val)
        -> void
{
    // the text_content is the attribute's value
    set_value(val);
}


auto dom::nodes::attr::set_value(
        const ext::string& val)
        -> void
{
    // setting the value calls a helper set method
    helpers::attributes::set_existing_attribute_value(this, val);
}


auto dom::nodes::attr::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<attr>{isolate}
            .inherit<node>()
            .var("namespaceURI", &attr::namespace_uri, true)
            .var("prefix", &attr::prefix, true)
            .var("localName", &attr::local_name, true)
            .var("name", &attr::name, true)
            .var("value", &attr::value)
            .var("ownerElement", &attr::owner_element, true);
}
