#include "html_link_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_link_element::html_link_element()
        : html_element()
{
    // constrain the property values
    rel.constrain_values({"alternate", "dns-prefetch", "icon", "manifest", "modulepreload", "next", "pingback", "preconnect", "prefetch", "preload", "prerender", "search", "stylesheet", "canonical"});
}