#pragma once
#ifndef SBROWSER_MUTATION_OBSERVERS_HPP
#define SBROWSER_MUTATION_OBSERVERS_HPP

#include <functional>

#include <ext/string.hpp>
#include <ext/vector.hpp>

namespace dom {
    namespace helpers {struct mutation_observers;}
    namespace nodes {
        class event_target;
        class node;
    }
}

namespace html::elements {class html_media_element;}


struct dom::helpers::mutation_observers {
private:
    using steps_t = std::function<void()>;

public:
    static void notify_mutation_observers();
    static void queue_microtask(steps_t&& steps);
    static void queue_mutation_record(ext::cstring& type, nodes::event_target* target, ext::cstring& name, ext::cstring& namespace_, ext::cstring& old_value, ext::vector<nodes::node*>& added_nodes, ext::vector<nodes::node*>& removed_nodes, nodes::node* previous_sibling, nodes::node* next_sibling);
    static void queue_tree_mutation_record(nodes::event_target* target, ext::vector<nodes::node*> added_nodes, ext::vector<nodes::node*> removed_nodes, nodes::node* previous_sibling, nodes::node* next_sibling);
    static void queue_mutation_observers_microtask();
    static void queue_task(/* TODO */) {/* TODO */};
    static void queue_global_task(/* TODO */) {/* TODO */};
    static void queue_element_task(/* TODO */) {/* TODO */};
    static void queue_media_element_task(html::elements::html_media_element* element, steps_t&& steps);
};


#endif //SBROWSER_MUTATION_OBSERVERS_HPP
