#include "html_script_element.hpp"

#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>


html::elements::html_script_element::html_script_element()
{
    // set the custom accessor methods
    text.getter = [this] {return get_text();};
    text.setter = [this](auto&& PH1) {set_text(std::forward<decltype(PH1)>(PH1));};

    // set the attribute's values
    m_parser_document = nullptr;
}


auto html::elements::html_script_element::supports(
        const ext::string& type)
        -> bool
{
    // supported types are 'classic' and 'modules'
    return type == "classic" or type == "module";
}


auto html::elements::html_script_element::get_text() const
        -> ext::string
{
    // get the child text content that forms the script
    return dom::helpers::trees::child_text_content(this);
}


auto html::elements::html_script_element::set_text(
        const ext::string& val)
        -> void
{
    // replace all the child text content with the new script
    dom::helpers::node_internals::string_replace_all(val, this);
}


auto html::elements::html_script_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_script_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .function("supports", &html_script_element::supports) // TODO : static
            .var("src", &html_script_element::src)
            .var("type", &html_script_element::type)
            .var("cross_origin", &html_script_element::cross_origin)
            .var("text", &html_script_element::text)
            .var("integrity", &html_script_element::integrity)
            .var("referrer_policy", &html_script_element::referrer_policy)
            .var("no_module", &html_script_element::no_module)
            .var("async", &html_script_element::async)
            .var("defer", &html_script_element::defer)
            .var("blocking", &html_script_element::blocking)
            .auto_wrap_objects();
}
