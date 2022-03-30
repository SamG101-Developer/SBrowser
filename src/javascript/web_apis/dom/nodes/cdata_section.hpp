#pragma once
#ifndef SBROWSER_CDATA_SECTION_HPP
#define SBROWSER_CDATA_SECTION_HPP

#include <dom/nodes/text.hpp>

namespace dom::nodes {class cdata_section;}


class dom::nodes::cdata_section : public text
{
public: constructors
    cdata_section();

public: cpp_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CDATA_SECTION_HPP
