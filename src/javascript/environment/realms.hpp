#ifndef SBROWSER_REALMS_HPP
#define SBROWSER_REALMS_HPP

#include <ext/string.hpp>

#include <v8.h>
#include <v8pp/convert.hpp>

namespace javascript::realms {
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

    template <typename T>
    inline T&
    get(ext::string&& attribute_name) {
        return v8pp::convert<T>::from_v8(m_context->Global()->Get(m_context, v8pp::convert<ext::string>::to_v8(v8::Isolate::GetCurrent(), attribute_name)));
    };

    template <typename T>
    inline void
    set(ext::string&& attribute_name, T new_value) {
        v8pp::convert<T>::from_v8(m_context->Global()->Get(m_context, v8pp::convert<ext::string>::to_v8(v8::Isolate::GetCurrent(), attribute_name))) = new_value;
    };

private:
    v8::Local<v8::Context> m_context;
};


javascript::realms::realm javascript::realms::relevant_agent() {return realm{v8::Isolate().GetCurrentContext()};}


#endif //SBROWSER_REALMS_HPP
