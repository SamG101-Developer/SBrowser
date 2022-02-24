#ifndef SBROWSER_REALMS_HPP
#define SBROWSER_REALMS_HPP

#include <ext/iterables/string.hpp>

#include <v8.h>
#include <v8pp/convert.hpp>

namespace javascript::realms
{
    class realm;

    realm relevant_agent();
    realm surrounding_agent();
    realm current_agent();

    realm current_global_object();
    realm relevant_global_object();
}


class javascript::realms::realm {
public:
    explicit realm(v8::Local<v8::Context> context): m_context(context) {};

    template <typename T> auto get(ext::string&& attribute_name) const -> T;
    template <typename T> auto set(ext::string&& attribute_name, T new_value) -> void;

private:
    v8::Local<v8::Context> m_context;
};


template <typename T>
auto javascript::realms::realm::get(ext::string&& attribute_name) const -> T
{
    // get the v8 object from the javascript context and cast it to its c++ type
    auto v8_object = m_context->Global()->Get(m_context, attribute_name).ToLocalChecked();

    // return the cast value
    return v8pp::convert<T>::from_v8(v8::Isolate::GetCurrent(), v8_object);
}


template <typename T>
auto javascript::realms::realm::set(ext::string&& attribute_name, T new_value) -> void
{
    // get the c++ object from the javascript context and cast it to its v8 type
    auto v8_value = v8pp::convert<T>::to_v8(v8::Isolate::GetCurrent(), new_value);

    // update the value in javascript
    m_context->Global()->Set(m_context, attribute_name, v8_value);
}


//


javascript::realms::realm javascript::realms::relevant_agent()
{
    return realm{v8::Isolate::GetCurrent()->GetCurrentContext()};
}


#endif //SBROWSER_REALMS_HPP
