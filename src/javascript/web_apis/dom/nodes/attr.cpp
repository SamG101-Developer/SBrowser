#include "attr.hpp"

#include <dom/helpers/attributes.hpp>


dom::nodes::attr::attr() : node()
{
    // set the custom accessors
    node_value.getter       = [this] {return get_node_value(); };
    text_content.getter     = [this] {return get_text_content(); };
    m_qualified_name.getter = [this] {return get_m_qualified_name(); };

    value.setter        = [this](auto&& PH1) {set_value(std::forward<decltype(PH1)>(PH1));};
    node_value.setter   = [this](auto&& PH1) {set_node_value(std::forward<decltype(PH1)>(PH1));};
    text_content.setter = [this](auto&& PH1) {set_text_content(std::forward<decltype(PH1)>(PH1));};

    // set the properties
    node_type << ATTRIBUTE_NODE;
}


auto dom::nodes::attr::get_node_value() const -> ext::string
{
    // the node_value is the attribute's value
    return value;
}


auto dom::nodes::attr::get_text_content() const -> ext::string
{
    // the text_content is the attribute's value
    return value;
}


auto dom::nodes::attr::get_m_qualified_name() const -> ext::string
{
    // the qualified name is the prefix and local name joined with a colon
    return prefix + ext::string{":"} + local_name;
}


void dom::nodes::attr::set_node_value(ext::cstring& val)
{
    // the node_value is the attribute's value
    set_value(val);
}


void dom::nodes::attr::set_text_content(ext::cstring& val)
{
    // the text_content is the attribute's value
    set_value(val);
}


void dom::nodes::attr::set_value(ext::cstring& val)
{
    // setting the value calls a helper set method
    helpers::attributes::set_existing_attribute_value(this, val);
}


auto dom::nodes::attr::v8(v8::Isolate* isolate) const -> ext::any
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
