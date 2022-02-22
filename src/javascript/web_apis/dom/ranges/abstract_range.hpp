#pragma once
#ifndef SBROWSER_ABSTRACT_RANGE_HPP
#define SBROWSER_ABSTRACT_RANGE_HPP

#include <ext/macros/decorators.hpp>
#include <ext/properties/dom_property.hpp>

#include <dom_object.hpp>

namespace dom {
    namespace nodes {class node;}
    namespace ranges {class abstract_range;}
}


class dom::ranges::abstract_range : public dom_object {
public constructors:
    abstract_range();

public properties:
    ext::dom_property<bool, _F> collapsed;
    ext::dom_property<nodes::node*, _F> start_container;
    ext::dom_property<nodes::node*, _F> end_container;
    ext::dom_property<unsigned long, _F> start_offset;
    ext::dom_property<unsigned long, _F> end_offset;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_ABSTRACT_RANGE_HPP
