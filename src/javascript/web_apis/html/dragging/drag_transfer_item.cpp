#include "drag_transfer_item.hpp"

#include <dom/helpers/mutation_observers.hpp>


html::dragging::drag_transfer_item::drag_transfer_item()
{
    // constrain the property values
    kind.constrain_values({"READ-WRITE", "READ-ONLY", "PROTECTED"});
    type.constrain_values({"text"});
}


auto html::dragging::drag_transfer_item::get_as_string(
        const function_callback_t& callback)
        -> void
{
    if (kind != "READ-WRITE" or kind != "READ-ONLY")
        return;

    if (type != "text")
        return;

    // TODO : dom::helpers::mutation_observers::queue_task()
}
