#pragma once
#ifndef SBROWSER_MUTATION_OBSERVER_HPP
#define SBROWSER_MUTATION_OBSERVER_HPP

#include <functional>
#include <queue>

#include <ext/macros/decorators.hpp>
#include <ext/iterables/map.hpp>
#include <ext/iterables/vector.hpp>

namespace dom {
    namespace helpers {class mutation_observers;}
    namespace nodes {class node;}
    namespace mutations {
        class mutation_observer;
        struct mutation_record;
    }
}


class dom::mutations::mutation_observer {
friends
    friend struct helpers::mutation_observers;

public: aliases
    using mutation_callback = std::function<void(ext::vector<mutation_record*>*, mutation_observer*)>;

public: constructors
    mutation_observer(mutation_callback&& callback);

public: methods
    void observe(nodes::node* target, ext::string_any_map&& options = {});
    void disconnect();
    ext::vector<mutation_record*> take_records();

private: internal_properties
    mutation_callback m_callback;
    ext::vector<nodes::node*>* m_node_list;
    std::queue<mutation_record*>* m_record_queue;
};


#endif //SBROWSER_MUTATION_OBSERVER_HPP
