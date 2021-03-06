 #include "shadow_root.hpp"

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/shadows.hpp>


dom::nodes::shadow_root::shadow_root()
        : document_fragment()
        , mixins::document_or_shadow_root<dom::nodes::shadow_root>()
{
    // set the property's values
    delegates_focus << false;
}


auto dom::nodes::shadow_root::get_the_parent(
        events::event* event)
        -> dom::nodes::event_target*
{
    // set the invocation target to the paths first invocation target, and the shadow root to its shadow root
    auto* invocation_target = event->path->front()->invocation_target;
    auto* shadow_root = helpers::shadows::shadow_root(dynamic_cast<node*>(invocation_target));

    // return this shadow root's host if this shadow root is the target's shadow root and the event isn't composed
    return not event->composed and shadow_root == this
            ? nullptr
            : ext::property_dynamic_cast<event_target*>(host);
}


auto dom::nodes::shadow_root::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<shadow_root>{isolate}
            .inherit<document_fragment>()
            .inherit<mixins::document_or_shadow_root<shadow_root>>()
            .var("mode", &shadow_root::mode, true)
            .var("delegatesFocus", &shadow_root::delegates_focus, true)
            .var("slotAssignment", &shadow_root::slot_assignment, true)
            .var("host", &shadow_root::host, true)
            .auto_wrap_objects();
}
