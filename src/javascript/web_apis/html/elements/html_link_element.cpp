#include "html_link_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_link_element::html_link_element() = default;


auto html::elements::html_link_element::set_rel(
        const ext::string& val)
        -> void
{
    if (m_link_types.contains(val))
        rel << val;
}
