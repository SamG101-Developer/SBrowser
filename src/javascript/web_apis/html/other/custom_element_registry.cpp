#include "custom_element_registry.hpp"


auto html::other::custom_element_registry::define(
        const ext::string& name,
        const html_element_constructor_t& constructor,
        const ext::string_any_map& options)
        -> void
{
    // TODO : ctor check
}
