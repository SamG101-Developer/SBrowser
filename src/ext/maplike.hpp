#ifndef SBROWSER_MAPLIKE_HPP
#define SBROWSER_MAPLIKE_HPP

namespace ext {template <typename K, typename V> class maplike;}

#include <ext/map.hpp>
#include <ext/property.hpp>

namespace v8pp {template <typename T, typename Enable> class convert;}


template <typename K, typename V>
class ext::maplike final
{
public friends:
    friend struct v8pp::convert<ext::maplike<K, V>>;

public constructors:
    explicit maplike(ext::map<K, V>* linked_map);

public js_properties:
    ext::property<size_t> length;

public operators:
    virtual auto operator[](const K& key) -> V&;

protected cpp_properties:
    ext::map<K, V>* m_linked_map;

private accessors:
    [[nodiscard]] auto get_length() const -> size_t;
};


template <typename K, typename V>
ext::maplike<K, V>::maplike(
        map<K, V>* linked_map):
        m_linked_map(linked_map)
{
    // set the custom accessor methods
    length.getter = [this] {return get_length();};
}


template <typename K, typename V>
auto ext::maplike<K, V>::operator[](
        const K& key)
        -> V&
{
    // get the element in the list by key matching
    return m_linked_map->at(key);
}


template <typename K, typename V>
auto ext::maplike<K, V>::get_length() const
        -> size_t
{
    // get the length of the internal map
    return m_linked_map->length();
}


#endif //SBROWSER_MAPLIKE_HPP
