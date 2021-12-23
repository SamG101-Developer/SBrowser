#include "shadow_root.hpp"

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/shadows.hpp>


dom::nodes::shadow_root::shadow_root()
        : document_fragment()
        , mixins::document_or_shadow_root<dom::nodes::shadow_root>() {

    delegates_focus = false;
}


dom::nodes::event_target*
dom::nodes::shadow_root::get_the_parent(events::event* event) {

    auto* invocation_target = event->path->front()->invocation_target;
    auto* shadow_root = helpers::shadows::shadow_root(dynamic_cast<node*>(invocation_target));

    return not event->composed and shadow_root == this
            ? nullptr
            : ext::property_dynamic_cast<event_target*>(host);
}
