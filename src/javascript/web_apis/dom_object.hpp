#ifndef SBROWSER_DOM_OBJECT_HPP
#define SBROWSER_DOM_OBJECT_HPP

#include <ext/macros/decorators.hpp>
#include <ext/types/any.hpp>
#include <ext/types/property.hpp>

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
    virtual ext::any&& v8(v8::Isolate* isolate) const = 0;
};


#endif //SBROWSER_DOM_OBJECT_HPP
