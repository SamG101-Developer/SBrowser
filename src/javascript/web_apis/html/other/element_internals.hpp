#pragma once
#ifndef SBROWSER_ELEMENT_INTERNALS_HPP
#define SBROWSER_ELEMENT_INTERNALS_HPP

#include <dom_object.hpp>
#include <html/mixins/labelable.hpp>
#include <html/mixins/validatable.hpp>

namespace dom::nodes {class shadow_root;}
namespace html::elements {class html_element;}
namespace html::other {class element_internals;}


class html::other::element_internals
        : public virtual dom_object
        , public mixins::labelable<element_internals>
        , public mixins::validatable<element_internals>
{
public constructors:
    element_internals();

public js_methods:
    auto set_validity(const ext::map<ext::string, bool>& flags = {}, const ext::string& message = "", elements::html_element* anchor = nullptr);
    auto set_form_value(const ext::string& value = "", const ext::string& state = "") -> void;

public js_properties:
    ext::html_property<dom::nodes::shadow_root*, _F> shadow_root_node;

private cpp_properties:
    elements::html_element* m_target_element;

private accessors:
    auto get_shadow_root() const -> dom::nodes::shadow_root*;
};


#endif //SBROWSER_ELEMENT_INTERNALS_HPP
