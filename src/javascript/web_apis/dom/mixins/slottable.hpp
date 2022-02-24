#pragma once
#ifndef SBROWSER_SLOTTABLE_HPP
#define SBROWSER_SLOTTABLE_HPP

#include <ext/iterables/string.hpp>
#include <ext/properties/dom_property.hpp>
#include <dom_object.hpp>

namespace dom
{
    namespace mixins {template <typename T> class slottable;}
    namespace helpers {struct shadows;}
}
namespace html::elements
{
    class html_slot_element;
}


template <typename T>
class dom::mixins::slottable : virtual public dom_object
{
public: friends
    friend struct helpers::shadows;

public: constructors
    slottable();

public: properties
    ext::dom_property<html::elements::html_slot_element*, _F> assigned_slot;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private: internal_properties
    ext::string m_name;
    html::elements::html_slot_element* m_manual_slot_assignment;

private: accessors
    auto get_assigned_slot() const -> html::elements::html_slot_element*;
};


#endif //SBROWSER_SLOTTABLE_HPP
