#pragma once
#ifndef SBROWSER_CDATA_SECTION_HPP
#define SBROWSER_CDATA_SECTION_HPP

#include <dom/nodes/text.hpp>

namespace dom::nodes {class cdata_section;}


class dom::nodes::cdata_section : public dom::nodes::text {
public constructors:
    cdata_section();
};


#endif //SBROWSER_CDATA_SECTION_HPP
