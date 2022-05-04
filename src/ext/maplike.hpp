#ifndef SBROWSER_MAPLIKE_HPP
#define SBROWSER_MAPLIKE_HPP

#include <dom_object.hpp>

namespace ext {template <typename K, typename V> class maplike;}

namespace ext {template <typename K, typename V> class map;}


template <typename K, typename V>
class ext::maplike : public virtual dom_object
{
public friends:
    friend struct v8pp::convert<ext::maplike<K, V>>;

public constructors:
    explicit maplike(ext::map<K, V>* linked_map);

public js_properties:
    ext::property<size_t> length;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

public operators:
    virtual auto operator[](const K& key) -> V&;

protected cpp_properties:
    ext::map<K, V>* m_linked_list;

private accessors:
    auto get_length() -> size_t;
};


#endif //SBROWSER_MAPLIKE_HPP
