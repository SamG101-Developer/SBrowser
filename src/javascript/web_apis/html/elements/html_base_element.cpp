#include "html_base_element.hpp"

#include <dom/nodes/document.hpp>
#include <html/helpers/custom_html_elements.hpp>


html::elements::html_base_element::html_base_element()
        : html_element()
{
    // set the custom accessors
    href.getter = [this] {return get_href();};

    // initialize html constructor with boilerplate code
    HTML_CONSTRUCTOR
}


auto html::elements::html_base_element::get_href() const -> ext::string
{
    // get the owner document and create the url object
    dom::nodes::document* document = owner_document;
    ext::string url;

    // get the href and put it into the url object, and parse it into the url record
    href >> url;
    auto url_record = url::helpers::parsing::parse_url(url, document->m_fallback_base_url, document->m_encoding);

    // if the url record is correctly constructed (no errors), then return the serialization of it, otherwise the url
    return not url_record ? url : url::helpers::serialize_url(url_record);
}
