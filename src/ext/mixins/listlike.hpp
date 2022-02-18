#pragma once
#ifndef SBROWSER_LISTLIKE_HPP
#define SBROWSER_LISTLIKE_HPP

#include <ext/types/property.hpp>

namespace ext {
    template <typename T> class vector;
    template <typename T> class listlike;
}

template <typename T>
class ext::listlike {
public:
    listlike(ext::vector<T>* linked_list = {});

    virtual T operator[] (size_t index) {return m_linked_list->at(index);}
    virtual T operator[] (ext::cstring& index) {return nullptr;}

    ext::property<size_t> length;

protected:
    ext::vector<T>* m_linked_list;

private:
    size_t get_length() {return m_linked_list->length();}
};


template <typename T>
ext::listlike<T>::listlike(ext::vector<T>* linked_list) : m_linked_list(linked_list)
{
    // set the custom accessors
    length.get = std::bind(&listlike<T>::get_length, this);
}


#endif //SBROWSER_LISTLIKE_HPP
