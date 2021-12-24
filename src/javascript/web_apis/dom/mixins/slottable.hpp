#pragma once
#ifndef SBROWSER_SLOTTABLE_HPP
#define SBROWSER_SLOTTABLE_HPP

#include <ext/property.hpp>

namespace dom::mixins {template <typename T> class slottable;}
namespace html::elements {class html_slot_element;}


template <typename T>
class dom::mixins::slottable {
public:
    slottable();

public:
    ext::dom_property<html::elements::html_slot_element*> assigned_slot;

private:
    ext::string m_name;
    html::elements::html_slot_element* m_manual_slot_assignment;

private:
    html::elements::html_slot_element* get_assigned_slot();
};


#endif //SBROWSER_SLOTTABLE_HPP
