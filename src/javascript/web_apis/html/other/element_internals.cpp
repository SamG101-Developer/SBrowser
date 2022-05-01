#include "element_internals.hpp"

#include <ext/type_traits.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/shadows.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <html/elements/html_element.hpp>

#include <html/mixins/form_associated.hpp>


html::other::element_internals::element_internals()
{
    // set the custom accessor methods
    shadow_root_node.getter = [this] {return get_shadow_root();};
}


auto html::other::element_internals::get_shadow_root() const
        -> dom::nodes::shadow_root*
{
    if (dom::helpers::shadows::is_shadow_host(m_target_element))
        return nullptr;

    if (not dom::helpers::shadows::shadow_root(m_target_element)->m_available_to_internals)
        return nullptr;

    return dom::helpers::shadows::shadow_root(m_target_element);
}


auto html::other::element_internals::set_form_value(
        const ext::string& value,
        const ext::string& state)
        -> void
{
    dom::helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "element must be form associated",
            [] {return not ext::is_mixin_base_of_v<html::mixins::form_associated, std::remove_pointer<decltype(m_target_element)>>;});

    auto form_associated_target_element = dynamic_cast<mixins::form_associated<elements::html_element>*>(m_target_element);

    // TODO
}
