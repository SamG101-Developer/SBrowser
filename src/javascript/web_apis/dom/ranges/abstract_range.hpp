#pragma once
#ifndef SBROWSER_ABSTRACT_RANGE_HPP
#define SBROWSER_ABSTRACT_RANGE_HPP

#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>

#include <dom_object.hpp>

namespace dom {
    namespace nodes {class node;}
    namespace ranges {class abstract_range;}
}


class dom::ranges::abstract_range : public dom_object {
public constructors:
    abstract_range();

public properties:
    ext::dom_property<bool> collapsed;
    ext::dom_property<nodes::node*> start_container;
    ext::dom_property<nodes::node*> end_container;
    ext::dom_property<unsigned long> start_offset;
    ext::dom_property<unsigned long> end_offset;

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;
};


#endif //SBROWSER_ABSTRACT_RANGE_HPP
