#include "html_output_element.hpp"

#include <dom/helpers/trees.hpp>
#include <dom/helpers/node_internals.hpp>


html::elements::html_output_element::html_output_element()
{
    // set the custom accessor methods
    default_value.getter = [this] {return get_default_value();};
    value.getter = [this] {return get_value();};

    default_value.setter = [this](auto&& PH1) {set_default_value(std::forward<decltype(PH1)>(PH1));};
    value.setter = [this](auto&& PH1) {set_value(std::forward<decltype(PH1)>(PH1));};

    // set the property values
    type << "output";
}


auto html::elements::html_output_element::get_default_value() const
        -> ext::string
{
    // return the default value override if it exists, otherwise the value of this element
    return not m_default_value_override.empty() ? m_default_value_override : value;
}


auto html::elements::html_output_element::get_value() const
        -> ext::string
{
    // return the descendant text content of this node
    return dom::helpers::trees::descendant_text_content(this);
}


auto html::elements::html_output_element::set_default_value(
        const ext::string& val)
        -> void
{
    // if there is no default value override, then replace all text with the new value
    if (m_default_value_override.empty())
    {
        dom::helpers::node_internals::string_replace_all(val, this);
        return;
    }

    // otherwise, set the default value override to the value
    m_default_value_override = val;
}


auto html::elements::html_output_element::set_value(
        const ext::string& val)
        -> void
{
    // set the default value override to the default value, then replace all text with the new value
    default_value >> m_default_value_override;
    dom::helpers::node_internals::string_replace_all(val, this);
}


auto html::elements::html_output_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_output_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::form_associated<html_output_element>>()
            .inherit<mixins::labelable<html_output_element>>()
            .inherit<mixins::validatable<html_output_element>>()
            .var("htmlFor", &html_output_element::html_for)
            .var("defaultValue", &html_output_element::default_value)
            .auto_wrap_objects();
}
