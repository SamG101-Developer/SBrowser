#ifndef SBROWSER_DOM_OBJECT_HPP
#define SBROWSER_DOM_OBJECT_HPP

#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <ext/any.hpp>

#include <v8pp/class.hpp>


class dom_object {
public: constructors
    dom_object() = default;
    dom_object(const dom_object&) = default;
    dom_object(dom_object&&) noexcept = default;
    dom_object& operator=(const dom_object&) = default;
    dom_object& operator=(dom_object&&) = default;

    virtual ~dom_object() = default;

public: internal_methods
    virtual auto v8(v8::Isolate* isolate) const -> ext::any = 0;
};


auto dom_object::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<dom_object>{isolate}.auto_wrap_objects();
}


template <typename ...Args, typename U>
auto multi_cast(U* cls) -> bool
{
    return (... ||(nullptr != dynamic_cast<Args>(cls)));
}


#endif //SBROWSER_DOM_OBJECT_HPP
