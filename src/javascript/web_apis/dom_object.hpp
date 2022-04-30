#ifndef SBROWSER_DOM_OBJECT_HPP
#define SBROWSER_DOM_OBJECT_HPP

#include <ext/any.hpp>
#include <ext/casting.hpp>
#include <ext/decorators.hpp>
#include <ext/equalities.hpp>
#include <ext/html_property.hpp>
#include <ext/string.hpp>
#include <ext/type_traits.hpp>
#include <ext/vector.hpp>

#include <javascript/interop/attribute_descriptors.hpp>

#include <v8pp/class.hpp>

namespace dom::events {class event;}
namespace dom::nodes {class document;}
namespace dom::nodes {class node;}


class dom_object {
public constructors:
    dom_object();
    dom_object(const dom_object&) = default;
    dom_object(dom_object&&) noexcept = default;
    dom_object& operator=(const dom_object&) = default;
    dom_object& operator=(dom_object&&) = default;

    virtual ~dom_object() = default;

public cpp_methods:
    virtual auto v8(v8::Isolate* isolate) const -> ext::any = 0;

protected cpp_methods:
    struct { // TODO : annotate parameters
        std::function<bool()> has_activation_behaviour;
        std::function<void(dom::events::event*)> activation_behaviour;
        std::function<void()> adopting_steps;
        std::function<void(const ext::string&, const ext::string&, const ext::string&, const ext::string&)> attribute_change_steps;
        std::function<void()> children_changed_steps;
        std::function<void(dom::nodes::node*, dom::nodes::document*, bool)> cloning_steps;
        std::function<void()> insertion_steps;
        std::function<void(dom::nodes::node* /* old_parent */)> removal_steps;

        std::function<void(const ext::string&)> page_visibility_steps;
    } m_behaviour;
};


dom_object::dom_object()
{
    // activation behaviour is assigned if the method isn't the default empty lambda TODO : test
    m_behaviour.has_activation_behaviour = [this] {return m_behaviour.activation_behaviour == [] (auto*){};};
}


auto dom_object::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    // expose the dom_object base class to v8 (not visible or constructable in JavaScript)
    return v8pp::class_<dom_object>{isolate}.auto_wrap_objects();
}


#endif //SBROWSER_DOM_OBJECT_HPP
