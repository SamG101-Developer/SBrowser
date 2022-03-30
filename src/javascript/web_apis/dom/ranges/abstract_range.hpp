#pragma once
#ifndef SBROWSER_ABSTRACT_RANGE_HPP
#define SBROWSER_ABSTRACT_RANGE_HPP

#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <dom_object.hpp>

namespace dom::nodes {class node;}
namespace dom::ranges {class abstract_range;}


class dom::ranges::abstract_range : virtual public dom_object
{
public constructors:
    abstract_range();

public js_properties:
    ext::dom_property<bool> collapsed;
    ext::dom_property<nodes::node*> start_container;
    ext::dom_property<nodes::node*> end_container;
    ext::dom_property<unsigned long> start_offset;
    ext::dom_property<unsigned long> end_offset;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_ABSTRACT_RANGE_HPP
