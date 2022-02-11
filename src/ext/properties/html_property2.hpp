#ifndef SBROWSER_HTML_PROPERTY2_HPP
#define SBROWSER_HTML_PROPERTY2_HPP

#include <ext/properties/dom_property2.hpp>

namespace ext {
    template <typename T, bool ce_reactions> struct html_property2;
}


template <typename T, bool ce_reactions>
struct ext::html_property2 : public dom_property2<T, ce_reactions> {
    // TODO -> only addition is calling the render method on T (requires T base of HtmlElement
};

#endif //SBROWSER_HTML_PROPERTY2_HPP
