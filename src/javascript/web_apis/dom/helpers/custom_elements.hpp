#pragma once
#ifndef SBROWSER_CUSTOM_ELEMENTS_HPP
#define SBROWSER_CUSTOM_ELEMENTS_HPP

#include <queue>
#include <ext/map.hpp>
#include <ext/string.hpp>

namespace dom {
    namespace helpers {struct custom_elements;}
    namespace internal {
        struct custom_element_definition;
        using lifecycle_callback = std::function<void()>;
    }

    namespace nodes {
        class document;
        class element;
    }
}


struct dom::helpers::custom_elements {
    static dom::nodes::element* create_an_element(nodes::document* document, ext::cstring& local_name, ext::cstring& namespace_, ext::cstring& prefix = "", ext::cstring& is = "", bool synchronous_custom_elements_flag = false);
    static void upgrade_element(internal::custom_element_definition* definition, nodes::element* element);
    static void try_to_upgrade_element(nodes::element* element);
    static internal::custom_element_definition* lookup_custom_element_definition(nodes::document* document, ext::cstring& local_name, ext::cstring& namespace_, ext::cstring& is);
    static void enqueue_element_on_appropriate_element_queue(nodes::element* element);
    static void enqueue_custom_element_callback_reaction(nodes::element* element, ext::cstring& callback_name, ext::vector<ext::string>&& args);
    static void enqueue_custom_element_upgrade_reaction(nodes::element* element, internal::custom_element_definition* definition);
    static void enqueue_custom_element_reaction(std::queue<nodes::element*>& element_queue);
    static bool is_valid_custom_element_name(ext::cstring& element_name);
    static bool is_custom_node(nodes::element* element);

    static nodes::element* element_interface(ext::cstring& local_name, ext::cstring& namespace_);
};


struct dom::internal::custom_element_definition {
    bool form_associated;
    bool disable_internals;
    bool disable_shadow;

    ext::string name;
    ext::string local_name;

    ext::vector<ext::string> observed_attributes;
    ext::vector<nodes::element*> construction_stack;
    nodes::element* constructor;

    ext::map<ext::string, internal::lifecycle_callback*> lifecycle_callbacks;
};


#endif //SBROWSER_CUSTOM_ELEMENTS_HPP
