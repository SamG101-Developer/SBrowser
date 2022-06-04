#pragma once
#ifndef SBROWSER_MUTATION_OBSERVER_HPP
#define SBROWSER_MUTATION_OBSERVER_HPP

#include <dom_object.hpp>
namespace dom::mutations {class mutation_observer;}

#include <functional>
#include <queue>
#include <ext/map.hpp>
#include <ext/vector.hpp>
namespace dom::helpers {class mutation_observers;}
namespace dom::mutations {struct mutation_record;}
namespace dom::nodes {class node;}


class dom::mutations::mutation_observer final : public virtual dom_object
{
public friends:
    friend struct helpers::mutation_observers;

public aliases:
    using mutation_callback = std::function<void(ext::vector<mutation_record*>*, mutation_observer*)>;

public constructors:
    mutation_observer();
    mutation_observer(mutation_callback&& callback);

public js_methods:
    auto observe(const nodes::node* target, ext::string_any_map_t&& options = {}) -> void;
    auto disconnect() -> void;
    auto take_records() -> ext::vector<mutation_record*>;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    mutation_callback m_callback;
    smart_pointer<ext::vector<smart_pointer<nodes::node    >>> m_node_list;
    smart_pointer<std::queue <smart_pointer<mutation_record>>> m_record_queue;
};


#endif //SBROWSER_MUTATION_OBSERVER_HPP
