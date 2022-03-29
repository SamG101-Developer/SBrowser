#include "html_body_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_body_element::html_body_element()
        : html_element()
{
    local_name = "body";
    HTML_CONSTRUCTOR
}
