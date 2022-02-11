#ifndef SBROWSER_HTML_PROPERTY_HPP
#define SBROWSER_HTML_PROPERTY_HPP

#include <ext/properties/dom_property.hpp>

namespace ext {
    template <typename T, bool ce_reactions> struct html_property;
}


template <typename T, bool ce_reactions>
struct ext::html_property : public dom_property<T, ce_reactions> {
    // TODO -> only addition is calling the render method on T (requires T base of HtmlElement
};

#endif //SBROWSER_HTML_PROPERTY_HPP
