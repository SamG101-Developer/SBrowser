#pragma once
#ifndef SBROWSER_SHADOW_ROOT_HPP
#define SBROWSER_SHADOW_ROOT_HPP

#include <dom/nodes/document_fragment.hpp>
#include <dom/mixins/document_or_shadow_root.hpp>

namespace dom::nodes {class shadow_root;}


class dom::nodes::shadow_root
        : public document_fragment
        , public mixins::document_or_shadow_root<dom::nodes::shadow_root>
{
public: friends
    friend class element;

public: constructors
    shadow_root();

public: properties
    ext::dom_property<ext::string, _F> mode;
    ext::dom_property<ext::string, _F> slot_assignment;
    ext::dom_property<bool, _F> delegates_focus;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected: internal_properties
    bool m_available_to_internals = false;

protected: internal_methods
    auto get_the_parent(events::event* event) -> event_target* override;
};


#endif //SBROWSER_SHADOW_ROOT_HPP
