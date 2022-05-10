#pragma once
#ifndef SBROWSER_LISTLIKE_HPP
#define SBROWSER_LISTLIKE_HPP

namespace ext {template <typename T> class listlike;}

#include <ext/property.hpp>
#include <ext/vector.hpp>

namespace v8pp {template <typename T, typename Enable> class convert;}


template <typename T>
class ext::listlike
{
public friends:
    friend struct v8pp::convert<ext::listlike<T>>;

public constructors:
    explicit listlike(ext::vector<T>* linked_list);

public js_properties:
    ext::property<size_t> length;

public operators:
    virtual auto operator[](size_t index) -> T&;

protected cpp_properties:
    ext::vector<T>* m_linked_list;

private accessors:
    [[nodiscard]] auto get_length() const -> size_t;
};


template <typename T>
ext::listlike<T>::listlike(
        ext::vector<T>* linked_list)
        : m_linked_list(linked_list)
{
    // set the custom accessor methods
    length.getter = [this] {return get_length();};
}


template <typename T>
auto ext::listlike<T>::operator[](
        const size_t index)
        -> T&
{
    // get the indexth element in the list by iterator
    return m_linked_list->at(m_linked_list->begin() + index);
}


template <typename T>
auto ext::listlike<T>::get_length() const
        -> size_t
{
    // get the length of the internal list
    return m_linked_list->length();
}


#endif //SBROWSER_LISTLIKE_HPP
