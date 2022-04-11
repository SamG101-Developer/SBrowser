#include "html_script_element.hpp"

#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>


html::elements::html_script_element::html_script_element()
{
    // set the custom accessors
    text.getter = [this] {return get_text();};
    text.setter = [this](auto&& PH1) {set_text(std::forward<decltype(PH1)>(PH1));};

    // set the attributes
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
