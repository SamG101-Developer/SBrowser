#ifndef SBROWSER_REALMS_HPP
#define SBROWSER_REALMS_HPP

#include <ext/string.hpp>

namespace dom::nodes {class window;}

#include <v8.h>
#include <v8pp/convert.hpp>


// agent -> isolate
// realm -> context
// global object, settings object contained in realm

namespace dom::nodes {class node;}


namespace javascript::realms
{
    class realm;
    using global_object_t = dom::nodes::window;
    class settings_object_t;

    realm relevant_realm(dom::nodes::node* node);
    realm surrounding_realm(dom::nodes::node* node);
    realm current_realm(dom::nodes::node* node);
    realm entry_realm();
    realm incumbent_realm();
}


class javascript::realms::realm {
public constructors:
    explicit realm(const v8::Local<v8::Context> context): m_context(context) {};

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
