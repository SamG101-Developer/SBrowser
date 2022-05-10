#ifndef SBROWSER_MAPLIKE_HPP
#define SBROWSER_MAPLIKE_HPP

namespace ext {template <typename K, typename V> class maplike;}

#include <ext/property.hpp>

namespace ext {template <typename K, typename V> class map;}
namespace v8pp {template <typename T> class convert;}


template <typename K, typename V>
class ext::maplike
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
    ext::map<K, V>* m_linked_list;

private accessors:
    auto get_length() -> size_t;
};


#endif //SBROWSER_MAPLIKE_HPP
