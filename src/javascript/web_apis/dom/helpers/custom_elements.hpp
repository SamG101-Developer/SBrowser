#pragma once
#ifndef SBROWSER_CUSTOM_ELEMENTS_HPP
#define SBROWSER_CUSTOM_ELEMENTS_HPP

#include <queue>
#include <ext/iterables/map.hpp>
#include <ext/iterables/string.hpp>

namespace dom
{
    namespace helpers {struct custom_elements;}
    namespace internal
    {
        struct custom_element_definition;
        using lifecycle_callback = std::function<void()>;
    }
    namespace nodes
    {
        class document;
        class element;
    }
}


struct dom::helpers::custom_elements
{
    static auto create_an_element(nodes::document* document, ext::cstring& local_name, ext::cstring& namespace_, ext::cstring& prefix = "", ext::cstring& is = "", const bool synchronous_custom_elements_flag = false) -> dom::nodes::element*;
    static auto upgrade_element(internal::custom_element_definition* definition, const nodes::element* element) -> void;
    static auto try_to_upgrade_element(const nodes::element* element) -> void;
    static auto lookup_custom_element_definition(const nodes::document* document, ext::cstring& local_name, ext::cstring& namespace_, ext::cstring& is) -> internal::custom_element_definition*;

    static auto enqueue_element_on_appropriate_element_queue(const nodes::element* element) -> void;
    static auto enqueue_custom_element_callback_reaction(const nodes::element* element, ext::cstring& callback_name, ext::vector<ext::string>&& args) -> void;
    static auto enqueue_custom_element_upgrade_reaction(const nodes::element* element, internal::custom_element_definition* definition) -> void;
    static auto enqueue_custom_element_reaction(std::queue<nodes::element*>& element_queue) -> void;

    static auto is_valid_custom_element_name(ext::cstring& element_name) -> bool;
    static auto is_custom_node(const nodes::element* element) -> bool;
};


struct dom::internal::custom_element_definition
{
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
