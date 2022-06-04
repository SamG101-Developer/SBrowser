#include "slottable.hpp"

#include <dom/helpers/shadows.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>

#include <html/elements/html_slot_element.hpp>


template <typename T>
dom::mixins::slottable<T>::slottable()
{
    // set the custom accessor methods
    bind_get(assigned_slot, get_assigned_slot);
}


template <typename T>
auto dom::mixins::slottable<T>::get_assigned_slot(
        ) const
        -> smart_pointer<html::elements::html_slot_element>
{
    // return the found slot from the helper function find_slot, after converting this into a node pointer
    auto* base = static_cast<const T*>(this);
    auto* slot = helpers::shadows::find_slot(base, true);
    return smart_pointer<html::elements::html_slot_element>(slot);
}


template <typename T>
auto dom::mixins::slottable<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<slottable<dom::nodes::node>>{isolate}
            .var("assignedSlot", &slottable<dom::nodes::node>::assigned_slot)
            .auto_wrap_objects();
}


template class dom::mixins::slottable<dom::nodes::element>;
template class dom::mixins::slottable<dom::nodes::text>;
