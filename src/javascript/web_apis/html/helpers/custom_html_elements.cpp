#include "custom_html_elements.hpp"

#include <ext/macros/string_switch.hpp>
#include <ext/iterables/vector.hpp>

#include <dom/helpers/custom_elements.hpp>
#include <html/elements/html_unknown_element.hpp>
#include <html/elements/html_pre_element.hpp>


auto html::helpers::custom_html_elements::element_interface(
        ext::cstring& local_name,
        ext::cstring& namespace_)
        -> elements::html_element*
{
    if (ext::string_vector{"acronym", "basefont", "big", "center", "nobr", "noembed", "noframes", "plaintext", "rb", "rtc", "strike", "tt"}.contains(local_name))
        return new html::elements::html_element{};

    else if (ext::string_vector{"listing", "xmp"}.contains(local_name))
        return new html::elements::html_pre_element{};

    else if (elements::html_element* element = local_name_to_object(local_name)) // TODO : html specs then other specs
        return element;

    else if (dom::helpers::custom_elements::is_valid_custom_element_name(local_name))
        return new html::elements::html_element{};

    return new html::elements::html_unknown_element{};
}
