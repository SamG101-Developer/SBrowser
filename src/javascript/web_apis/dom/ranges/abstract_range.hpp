#pragma once
#ifndef SBROWSER_ABSTRACT_RANGE_HPP
#define SBROWSER_ABSTRACT_RANGE_HPP

#include <ext/decorators.hpp>
#include <ext/property.hpp>

namespace dom {
    namespace nodes {class node;}
    namespace ranges {class abstract_range;}
}


class dom::ranges::abstract_range {
public constructors:
    abstract_range();

public properties:
    ext::dom_property<bool> collapsed;
    ext::dom_property<nodes::node*> start_container;
    ext::dom_property<nodes::node*> end_container;
    ext::dom_property<unsigned long> start_offset;
    ext::dom_property<unsigned long> end_offset;
};


#endif //SBROWSER_ABSTRACT_RANGE_HPP
