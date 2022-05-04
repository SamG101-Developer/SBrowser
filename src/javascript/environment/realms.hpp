#ifndef SBROWSER_REALMS_HPP
#define SBROWSER_REALMS_HPP

#include <ext/string.hpp>

#include <dom_object.hpp>
#include <v8.h>
#include <v8pp/convert.hpp>


// agent -> isolate
// realm -> context
// global object, settings object contained in realm


class dom_object;
namespace dom::nodes {class node;}
namespace dom::nodes {class window;}

namespace url {class url;}


namespace javascript::realms
{
    class realm;
    struct settings_object_t {};
    using global_object_t = dom::nodes::window;

    auto relevant_realm(dom_object* node) -> realm;
    auto surrounding_realm(dom_object* node) -> realm;
    auto current_realm(dom_object* node) -> realm;
    auto entry_realm() -> realm;
    auto incumbent_realm() -> realm;
}


class javascript::realms::realm
{
public constructors:
    explicit realm(const v8::Local<v8::Context> context) : m_context(context) {};

public cpp_methods:
    template <typename T> auto get(ext::string&& attribute_name) const -> T;
    template <typename T> auto set(ext::string&& attribute_name, T new_value) -> void;

public cpp_properties:
    global_object_t* global_object;
    settings_object_t* settings_object;

private cpp_properties:
    v8::Local<v8::Context> m_context;
};


template <typename T>
auto javascript::realms::realm::get(ext::string&& attribute_name) const -> T
{
    // get the v8 object from the javascript context and cast it to its c++ type
    auto v8_object = m_context->Global()->Get(
            m_context,
            v8pp::convert<ext::string>::to_v8(m_context->GetIsolate(), std::forward<ext::string>(attribute_name))).ToLocalChecked();

    // return the cast value
    return v8pp::convert<T>::from_v8(v8::Isolate::GetCurrent(), v8_object);
}


template <typename T>
auto javascript::realms::realm::set(ext::string&& attribute_name, T new_value) -> void
{
    // get the c++ object from the javascript context and cast it to its v8 type
    auto v8_value = v8pp::convert<T>::to_v8(v8::Isolate::GetCurrent(), new_value);

    // update the value in javascript
    m_context->Global()->Set(
            m_context,
            std::forward<ext::string>(attribute_name), v8_value);
}


#endif //SBROWSER_REALMS_HPP
