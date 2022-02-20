#pragma once
#ifndef SBROWSER_LISTLIKE_HPP
#define SBROWSER_LISTLIKE_HPP

#include <ext/properties/property.hpp>

namespace ext {
    template <typename T> class vector;
    template <typename T> class listlike;
}

template <typename T>
class ext::listlike {
public:
    listlike(ext::vector<T>* linked_list = {});

    virtual func operator[] (size_t index) -> T&;
    virtual func operator[] (ext::cstring& index) -> T&;

    ext::property<size_t> length;

protected:
    ext::vector<T>* m_linked_list;

private:
    func get_length() -> size_t;
};


template <typename T>
ext::listlike<T>::listlike(ext::vector<T>* linked_list) : m_linked_list(linked_list)
{
    // set the custom accessors
    length.get = std::bind(&listlike<T>::get_length, this);
}


template <typename T>
func ext::listlike<T>::operator[](size_t index) -> T&
{
    // use the [size_t] operator as getting an item by index
    return m_linked_list->at(index);
}


template <typename T>
func ext::listlike<T>::operator[](ext::cstring& index) -> T&
{
    // default behaviour of [ext::string] is to return a nullptr
    return nullptr;
}


template <typename T>
func ext::listlike<T>::get_length() -> size_t
{
    // get the length of the internal list
    return m_linked_list->length();
}


#endif //SBROWSER_LISTLIKE_HPP
