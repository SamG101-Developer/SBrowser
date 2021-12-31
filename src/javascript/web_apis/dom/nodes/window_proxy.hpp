#pragma once
#ifndef SBROWSER_WINDOW_PROXY_HPP
#define SBROWSER_WINDOW_PROXY_HPP

#include <ext/macros/decorators.hpp>
#include <v8.h>

namespace dom::nodes {
    class window;
    class window_proxy;
}


// Pre-wrapped JavaScript Class
class dom::nodes::window_proxy {
public internal_slot_methods:
    v8::Local<v8::Object> get_prototype_of();
    void set_prototype_of(v8::Local<v8::Object> V);
    constexpr v8::Local<v8::Boolean> is_extensible_prototype_of();
    constexpr v8::Local<v8::Boolean> prevent_extensions();
    v8::PropertyDescriptor get_own_property(v8::Local<v8::Integer> P);
    void define_property(v8::Local<v8::Integer> P, v8::Local<v8::String> D);
    v8::Local<v8::Object> get(v8::Local<v8::Integer> P, v8::Local<v8::Object> R);
    v8::Local<v8::Object> set(v8::Local<v8::Integer> P, v8::Local<v8::Object> V, v8::Local<v8::Object> R);
    v8::Local<v8::Boolean> delete_(v8::Local<v8::Number> P);
    v8::Local<v8::Array> own_property_keys();

public internal_slots:
    window* s_window;
};


#endif //SBROWSER_WINDOW_PROXY_HPP
