#pragma once
#ifndef SBROWSER_SHADOW_ROOT_HPP
#define SBROWSER_SHADOW_ROOT_HPP

#include <dom/nodes/document_fragment.hpp>

namespace dom::nodes {class shadow_root;}


class dom::nodes::shadow_root
        : public dom::nodes::document_fragment
        , public dom::mixins::document_or_shadow_root<dom::nodes::shadow_root> {

public constructors:
    shadow_root();

public properties:
    ext::dom_property<ext::string> mode;
    ext::dom_property<ext::string> slot_assignment;
    ext::dom_property<bool> delegates_focus;

protected internal_properties:
    bool m_available_to_internals = false;

protected internal_methods:
    event_target* get_the_parent(events::event* event) override;
};


#endif //SBROWSER_SHADOW_ROOT_HPP
