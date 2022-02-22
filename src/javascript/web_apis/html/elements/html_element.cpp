#include "html_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_element::html_element()
        : dom::nodes::element{}
{
    HTML_CONSTRUCTOR
}


auto html::elements::html_element::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<html_element>{isolate};
}
