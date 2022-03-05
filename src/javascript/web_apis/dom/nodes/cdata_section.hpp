#pragma once
#ifndef SBROWSER_CDATA_SECTION_HPP
#define SBROWSER_CDATA_SECTION_HPP

#include <dom/nodes/text.hpp>

namespace dom::nodes {class cdata_section;}


/*
 * https://dom.spec.whatwg.org/#interface-cdatasection
 * https://developer.mozilla.org/en-US/docs/Web/API/CDATASection
 *
 * The CDATASection interface represents a CDATA section that can be used within XML to include extended portions of
 * unescaped text. When inside a CDATA section, the symbols < and & don't need escaping as they normally do.
 */
class dom::nodes::cdata_section : public text
{
public: constructors
    cdata_section();

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CDATA_SECTION_HPP
