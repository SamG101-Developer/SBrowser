#include "slottable.hpp"

#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>

#include <dom/helpers/shadows.hpp>


template <typename T>
dom::mixins::slottable<T>::slottable()
{
    // set the custom accessors
    assigned_slot.getter = [this] {return get_assigned_slot();};

    // set the attributes
    m_manual_slot_assignment = nullptr;
}


template <typename T>
auto dom::mixins::slottable<T>::get_assigned_slot() const -> html::elements::html_slot_element*
{
    // return the found slot from the helper function find_slot, after converting this into a node pointer
    return helpers::shadows::find_slot((nodes::node*)reinterpret_cast<const nodes::node*>(this), true); // TODO : messy
}


template <typename T>
auto dom::mixins::slottable<T>::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<slottable<dom::nodes::node>>{isolate}
            .var("assignedSlot", &slottable<dom::nodes::node>::assigned_slot)
            .auto_wrap_objects();
}


template class dom::mixins::slottable<dom::nodes::element>;
template class dom::mixins::slottable<dom::nodes::text>;
