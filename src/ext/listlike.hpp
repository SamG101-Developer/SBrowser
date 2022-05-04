#pragma once
#ifndef SBROWSER_LISTLIKE_HPP
#define SBROWSER_LISTLIKE_HPP

#include <dom_object.hpp>

namespace ext {template <typename T> class listlike;}

namespace ext {template <typename T> class vector;}


template <typename T>
class ext::listlike : public virtual dom_object
{
public friends:
    friend struct v8pp::convert<ext::listlike<T>>;

public constructors:
    explicit listlike(ext::vector<T>* linked_list);

public js_properties:
    ext::property<size_t> length;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

public operators:
    virtual auto operator[](size_t index) -> T&;
    virtual auto operator[](const ext::string& index) -> T&;

protected cpp_properties:
    ext::vector<T>* m_linked_list;

private accessors:
    auto get_length() -> size_t;
};


template <typename T>
ext::listlike<T>::listlike(ext::vector<T>* linked_list) : m_linked_list(linked_list)
{
    // set the custom accessor methods
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
