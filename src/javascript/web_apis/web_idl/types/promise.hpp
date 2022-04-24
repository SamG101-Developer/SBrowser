#ifndef SBROWSER_PROMISE_HPP
#define SBROWSER_PROMISE_HPP

#include <ext/decorators.hpp>

#include <v8.h>
#include <v8pp/convert.hpp>

namespace dom{
    namespace nodes {class event_target;}
    namespace other {class dom_exception;}
}
namespace webidl::types {template <typename T> class promise;}


template <typename T>
class webidl::types::promise {
public constructors:
    promise();

public cpp_methods:
    auto resolve(dom::nodes::event_target* value = nullptr) -> promise<T>&;
    auto reject(const dom::other::dom_exception& value) -> promise<T>&;
    auto is_fulfilled() -> bool;

public operators:
    bool operator==(const promise<T>& other);

private: cpp_properties
    v8::Isolate* m_isolate;
    v8::Local<v8::Context> m_context;
    v8::Local<v8::Promise::Resolver> m_promise;
    bool m_fulfilled = false;
};


template <typename T>
webidl::types::promise<T>::promise()
        : m_isolate(v8::Isolate::GetCurrent())
        , m_context(v8::Isolate::GetCurrent()->GetCurrentContext())
        , m_promise(v8::Promise::Resolver::New(m_context).ToLocalChecked())
{}


template <typename T>
auto webidl::types::promise<T>::resolve(dom::nodes::event_target* value) -> promise<T>&
{
    m_fulfilled = m_promise->Resolve(m_context, v8pp::convert<decltype(value)>::to_v8(m_isolate, value)).ToChecked();
    return *this;
}


template <typename T>
auto webidl::types::promise<T>::reject(const dom::other::dom_exception& value) -> promise<T>&
{
    m_fulfilled = not m_promise->Reject(m_context, v8pp::convert<decltype(value)>::to_v8(m_isolate,value)).ToChecked();
    return *this;
}


template <typename T>
auto webidl::types::promise<T>::is_fulfilled() -> bool
{
    return m_fulfilled;
}


template <typename T>
bool webidl::types::promise<T>::operator==(const promise<T>& other)
{
    return this == &other;
}


#endif //SBROWSER_PROMISE_HPP
