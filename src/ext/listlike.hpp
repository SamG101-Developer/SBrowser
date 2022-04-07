#pragma once
#ifndef SBROWSER_LISTLIKE_HPP
#define SBROWSER_LISTLIKE_HPP

#include <ext/property.hpp>
#include <dom_object.hpp>


namespace ext {template <typename T> class vector;}
namespace ext {template <typename T> class listlike;}


template <typename T>
class ext::listlike : virtual public dom_object
{
public friends:
    friend v8pp::convert<ext::listlike<T>>;

public constructors:
    explicit listlike(ext::vector<T>* linked_list = {});

public operators:
    virtual auto operator[] (size_t index) -> T&;
    virtual auto operator[] (const ext::string& index) -> T&;

public js_properties:
    ext::property<size_t> length;

protected cpp_properties:
    ext::vector<T>* m_linked_list;

private cpp_properties:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto get_length() -> size_t;
};


template <typename T>
ext::listlike<T>::listlike(ext::vector<T>* linked_list) : m_linked_list(linked_list)
{
    // set the custom accessors
    length.getter = [this] {return get_length();};
}


template <typename T>
auto ext::listlike<T>::operator[](size_t index) -> T&
{
    // use the [size_t] operator as getting an item by index
    return m_linked_list->at(index);
}


template <typename T>
auto ext::listlike<T>::operator[](const ext::string& index) -> T&
{
    // default behaviour of [ext::string] is to return a nullptr
    return nullptr;
}


template <typename T>
auto ext::listlike<T>::get_length() -> size_t
{
    // get the length of the internal list
    return m_linked_list->length();
}


#endif //SBROWSER_LISTLIKE_HPP
