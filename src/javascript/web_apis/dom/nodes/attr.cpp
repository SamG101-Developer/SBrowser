#include "attr.hpp"

#include <dom/helpers/attributes.hpp>


dom::nodes::attr::attr() : node()
{
    // set the custom accessors
    node_value.get       = [this] {return get_node_value(); };
    text_content.get     = [this] {return get_text_content(); };
    m_qualified_name.get = [this] {return get_m_qualified_name(); };

    value.set        = [this](auto&& PH1) {set_value(std::forward<decltype(PH1)>(PH1));};
    node_value.set   = [this](auto&& PH1) {set_node_value(std::forward<decltype(PH1)>(PH1));};
    text_content.set = [this](auto&& PH1) {set_text_content(std::forward<decltype(PH1)>(PH1));};

    // set the properties
    node_type << ATTRIBUTE_NODE;
}


ext::string dom::nodes::attr::get_node_value() const
{
    // the node_value is the attribute's value
    return value;
}


ext::string dom::nodes::attr::get_text_content() const
{
    // the text_content is the attribute's value
    return value;
}


ext::string dom::nodes::attr::get_m_qualified_name() const
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
