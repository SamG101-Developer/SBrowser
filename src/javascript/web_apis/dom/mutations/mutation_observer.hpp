#pragma once
#ifndef SBROWSER_MUTATION_OBSERVER_HPP
#define SBROWSER_MUTATION_OBSERVER_HPP

#include <functional>
#include <queue>

#include <ext/decorators.hpp>
#include <ext/map.hpp>
#include <ext/vector.hpp>

#include <dom_object.hpp>

namespace dom
{
    namespace helpers {class mutation_observers;}
    namespace nodes {class node;}
    namespace mutations
    {
        class mutation_observer;
        struct mutation_record;
    }
}


/*
 * https://dom.spec.whatwg.org/#interface-mutationobserver
 * https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver
 *
 * The MutationObserver interface provides the ability to watch for changes being made to the DOM tree. It is designed
 * as a replacement for the older Mutation Events feature, which was part of the DOM3 Events specification.
 */
class dom::mutations::mutation_observer : virtual public dom_object
{
public: friends
    friend struct helpers::mutation_observers;

public: aliases
    using mutation_callback = std::function<void(ext::vector<mutation_record*>*, mutation_observer*)>;

public: constructors
    mutation_observer();
    mutation_observer(mutation_callback&& callback);

public: methods
    auto observe(const nodes::node* target, ext::string_any_map&& options = {}) -> void;
    auto disconnect() -> void;
    auto take_records() -> ext::vector<mutation_record*>;

public: internal_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private: internal_properties
    mutation_callback m_callback;
    ext::vector<nodes::node*>* m_node_list;
    std::queue<mutation_record*>* m_record_queue;
};


#endif //SBROWSER_MUTATION_OBSERVER_HPP
