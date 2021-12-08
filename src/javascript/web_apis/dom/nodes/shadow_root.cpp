#include "shadow_root.hpp"

#include <dom/events/event.hpp>
#include <dom/helpers/event_listening.hpp>


dom::nodes::shadow_root::shadow_root()
        : dom::nodes::document_fragment()
        , dom::mixins::document_or_shadow_root<dom::nodes::shadow_root> {

    delegates_focus = false;
}


dom::nodes::event_target*
dom::nodes::shadow_root::get_the_parent(events::event* event) {

    return not event->composed and dynamic_cast<shadow_root*>(helpers::trees::root(event->path->front()->invocation_target)) == this
            ? nullptr
            : host
}
