#include "bar_prop.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/window.hpp>

#include <html/helpers/browsing_context_internals.hpp>

#include <QtWidgets/QWidget>


html::other::bar_prop::bar_prop()
{
    // attach the qt functions

    // set the custom accessor methods
    visible.getter = [this] {return get_visible();};
}


auto html::other::bar_prop::get_visible() const
        -> bool
{
    auto* browsing_context = javascript::realms::relevant_realm(nullptr).global_object->document->m_browsing_context; // TODO : nullptr -> this
    return (not browsing_context) or browsing_context->top_level_browsing_context()->is_popup;
}
