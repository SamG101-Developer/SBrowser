 #include "shadow_root.hpp"

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/shadows.hpp>


dom::nodes::shadow_root::shadow_root()
        : document_fragment()
        , mixins::document_or_shadow_root<dom::nodes::shadow_root>()
{
    // set the properties
    delegates_focus << false;
}


dom::nodes::event_target*
dom::nodes::shadow_root::get_the_parent(events::event* event)
{
    // set the invocation target to the paths first invocation target, and the shadow root to its shadow root
    auto* invocation_target = event->path->front()->invocation_target;
    auto* shadow_root = helpers::shadows::shadow_root(dynamic_cast<node*>(invocation_target));

    // return this shadow root's host if this shadow root is the target's shadow root and the event isn't composed
    return not event->composed and shadow_root == this
            ? nullptr
            : ext::property_dynamic_cast<event_target*>(host);
}
