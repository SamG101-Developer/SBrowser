#pragma once
#ifndef SBROWSER_WINDOW_PROXY_HPP
#define SBROWSER_WINDOW_PROXY_HPP

#include <dom_object.hpp>


#include <v8.h>

namespace dom::nodes {class window;}
namespace dom::nodes {class window_proxy;}


// Pre-wrapped JavaScript Class
class dom::nodes::window_proxy : public virtual dom_object
{
public: constructors
    window_proxy();

    ~window_proxy() override; // TODO -> delete active document etc?

public: javascript_slot_methods
    auto get_prototype_of() -> v8::Local<v8::Object>;
    auto set_prototype_of(v8::Local<v8::Object> V) -> void;
    auto is_extensible_prototype_of() -> v8::Local<v8::Boolean>;
    auto prevent_extensions() -> v8::Local<v8::Boolean>;
    auto get_own_property(v8::Local<v8::Integer> P) -> v8::PropertyDescriptor;
    void define_property(v8::Local<v8::Integer> P, v8::Local<v8::String> D);
    auto get(v8::Local<v8::Integer> P, v8::Local<v8::Object> R) -> v8::Local<v8::Object>;
    auto set(v8::Local<v8::Integer> P, v8::Local<v8::Object> V, v8::Local<v8::Object> R) -> v8::Local<v8::Object>;
    auto delete_(v8::Local<v8::Number> P) -> v8::Local<v8::Boolean>;
    auto own_property_keys() -> v8::Local<v8::Array>;

public: javascript_slots
    window* s_window;

public: internal_methods
    ext::any v8(v8::Isolate* isolate) const override;
};


#endif //SBROWSER_WINDOW_PROXY_HPP
