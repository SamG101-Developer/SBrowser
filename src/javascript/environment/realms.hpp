#ifndef SBROWSER_REALMS_HPP
#define SBROWSER_REALMS_HPP

#include <ext/iterables/string.hpp>

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
    inline T
    get(ext::string&& attribute_name) const {
        return v8pp::convert<T>::from_v8(v8::Isolate::GetCurrent(), m_context->Global()->Get(m_context, attribute_name).ToLocalChecked());
    };

    template <typename T>
    inline void
    set(ext::string&& attribute_name, T new_value) {
        m_context->Global()->Set(m_context, attribute_name, v8pp::convert<T>::to_v8(v8::Isolate::GetCurrent(), new_value));
    };

private:
    v8::Local<v8::Context> m_context;
};


javascript::realms::realm javascript::realms::relevant_agent() {return realm{v8::Isolate::GetCurrent()->GetCurrentContext()};}


#endif //SBROWSER_REALMS_HPP
