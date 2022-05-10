#pragma once
#ifndef SBROWSER_LISTLIKE_HPP
#define SBROWSER_LISTLIKE_HPP

namespace ext {template <typename T> class listlike;}

namespace ext {template <typename T> class vector;}


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
    // use the [size_t] operator as getting an item by index
    return m_linked_list->at(index);
}


template <typename T>
auto ext::listlike<T>::get_length() const
        -> size_t
{
    // get the length of the internal list
    return m_linked_list->length();
}


#endif //SBROWSER_LISTLIKE_HPP
