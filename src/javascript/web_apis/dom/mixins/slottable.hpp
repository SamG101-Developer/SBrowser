#pragma once
#ifndef SBROWSER_SLOTTABLE_HPP
#define SBROWSER_SLOTTABLE_HPP

#include <dom_object.hpp>
namespace dom::mixins {template <typename T> class slottable;}

#include <ext/string.hpp>
namespace dom::helpers {struct shadows;}
namespace html::elements{class html_slot_element;}


template <typename T>
class dom::mixins::slottable : public virtual dom_object
{
public friends:
    friend struct helpers::shadows;
    friend class html::elements::html_slot_element;

public constructors:
    slottable();

public js_properties:
    ext::property<html::elements::html_slot_element*> assigned_slot;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    ext::string m_name;
    html::elements::html_slot_element* m_manual_slot_assignment = nullptr;

private accessors:
    [[nodiscard]] auto get_assigned_slot() const -> smart_pointer<html::elements::html_slot_element>;
};


#endif //SBROWSER_SLOTTABLE_HPP
