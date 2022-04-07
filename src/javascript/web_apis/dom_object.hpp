#ifndef SBROWSER_DOM_OBJECT_HPP
#define SBROWSER_DOM_OBJECT_HPP

#include <ext/any.hpp>
#include <ext/casting.hpp>
#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>

#include <javascript/interop/attribute_descriptors.hpp>

#include <v8pp/class.hpp>

namespace dom::events {class event;}
namespace dom::nodes {class node;}


class dom_object {
public constructors:
    dom_object() = default;
    dom_object(const dom_object&) = default;
    dom_object(dom_object&&) noexcept = default;
    dom_object& operator=(const dom_object&) = default;
    dom_object& operator=(dom_object&&) = default;

    virtual ~dom_object() = default;

public cpp_methods:
    virtual auto v8(v8::Isolate* isolate) const -> ext::any = 0;
    virtual auto activation_behaviour(dom::events::event* event) -> void {};
    auto has_activation_behaviour() -> bool;

protected cpp_methods:
    virtual auto insertion_steps() -> void {};
    virtual auto removal_steps(dom::nodes::node* old_parent = nullptr) -> void {};
};


auto dom_object::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    // expose the dom_object base class to v8 (not visible or constructable in JavaScript)
    return v8pp::class_<dom_object>{isolate}.auto_wrap_objects();
}


auto dom_object::has_activation_behaviour()
        -> bool
{
    // the object has activation behaviour if the method is overridden
    return std::is_same_v<decltype(&dom_object::activation_behaviour), decltype(&std::remove_pointer_t<decltype(this)>::activation_behaviour)>;
}




#endif //SBROWSER_DOM_OBJECT_HPP
