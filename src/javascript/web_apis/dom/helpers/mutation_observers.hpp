#pragma once
#ifndef SBROWSER_MUTATION_OBSERVERS_HPP
#define SBROWSER_MUTATION_OBSERVERS_HPP

#include <functional>

#include <ext/map.hpp>
#include <ext/string.hpp>
#include <ext/vector.hpp>

namespace dom::helpers {struct mutation_observers;}
namespace dom::internal {struct registered_observer;}
namespace dom::internal {struct transient_registered_observer;}
namespace dom::mutations {class mutation_observer;}
namespace dom::nodes {class event_target;}
namespace dom::nodes {class node;}
namespace html::elements {class html_element;}
namespace html::elements {class html_media_element;}


struct dom::helpers::mutation_observers final
{
private:
    using steps_t = std::function<void()>;

public:
    // notifications
    static auto notify_mutation_observers() -> void;

    // queues
    static auto queue_microtask(
            steps_t&& steps) -> void;

    static auto queue_mutation_record(
            const ext::string& type,
            const nodes::event_target* target,
            const ext::string& name,
            const ext::string& namespace_,
            const ext::string& old_value,
            const ext::vector<nodes::node*>& added_nodes,
            const ext::vector<nodes::node*>& removed_nodes,
            nodes::node* previous_sibling,
            nodes::node* next_sibling)
            -> void;

    static auto queue_tree_mutation_record(
            nodes::event_target* target,
            const ext::vector<nodes::node*>& added_nodes,
            const ext::vector<nodes::node*>& removed_nodes,
            nodes::node* previous_sibling,
            nodes::node* next_sibling)
            -> void;

    static auto queue_mutation_observers_microtask()
            -> void;

    static auto queue_task(
            steps_t&& steps)
            -> void;

    static auto queue_global_task(/* TODO */)
            -> void;

    static auto queue_element_task(
            html::elements::html_element* element,
            steps_t&& steps)
            -> void;

    static auto queue_media_element_task(
            html::elements::html_media_element* element,
            steps_t&& steps)
            -> void;
};


struct dom::internal::registered_observer
{
    mutations::mutation_observer* observer = nullptr;
    ext::string_any_map options;
};


struct dom::internal::transient_registered_observer : public registered_observer
{
    registered_observer* source = nullptr;
};


#endif //SBROWSER_MUTATION_OBSERVERS_HPP
