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
public friends:
    friend class element;

public constructors:
    shadow_root();

public js_properties:
    ext::dom_property<ext::string> mode;
    ext::dom_property<ext::string> slot_assignment;
    ext::dom_property<bool> delegates_focus;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_properties:
    bool m_available_to_internals = false;

protected cpp_methods:
    auto get_the_parent(events::event* event) -> event_target* override;
};


#endif //SBROWSER_SHADOW_ROOT_HPP
