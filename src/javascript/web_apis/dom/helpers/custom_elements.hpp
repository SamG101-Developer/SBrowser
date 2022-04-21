#pragma once
#ifndef SBROWSER_CUSTOM_ELEMENTS_HPP
#define SBROWSER_CUSTOM_ELEMENTS_HPP

#include <queue>
#include <ext/map.hpp>
#include <ext/string.hpp>

namespace dom::helpers {struct custom_elements;}
namespace dom::internal {struct custom_element_definition;}
namespace dom::nodes {class document;}
namespace dom::nodes {class element;}


struct dom::helpers::custom_elements final
{
    // custom element creation and upgrading
    template <typename T=dom::nodes::element>
    static auto create_an_element(
            nodes::document* document,
            const ext::string& local_name,
            const ext::string& namespace_,
            const ext::string& prefix = "",
            const ext::string& is = "",
            bool synchronous_custom_elements_flag = false)
            -> T*;

    static auto upgrade_element(
            internal::custom_element_definition* definition,
            nodes::element* element)
            -> void;

    static auto try_to_upgrade_element(
            const nodes::element* element)
            -> void;

    static auto lookup_custom_element_definition(
            const nodes::document* document,
            const ext::string& local_name,
            const ext::string& namespace_,
            const ext::string& is)
            -> internal::custom_element_definition*;

    // enqueue methods for custom elements
    static auto enqueue_element_on_appropriate_element_queue(
            const nodes::element* element)
            -> void;

    static auto enqueue_custom_element_callback_reaction(
            const nodes::element* element,
            const ext::string& callback_name,
            ext::string_vector&& args)
            -> void;

    static auto enqueue_custom_element_upgrade_reaction(
            const nodes::element* element,
            const internal::custom_element_definition* definition)
            -> void;

    static auto enqueue_custom_element_reaction(
            const std::queue<nodes::element*>& element_queue)
            -> void;

    // custom element checks
    static auto is_valid_custom_element_name(
            const ext::string& element_name)
            -> bool;

    static auto is_custom_node(
            const nodes::element* element)
            -> bool;
};


struct dom::internal::custom_element_definition
{
    using lifecycle_callback_t = std::function<void()>;
    using html_element_constructor_t = std::function<nodes::element()>;

    bool form_associated;
    bool disable_internals;
    bool disable_shadow;

    ext::string name;
    ext::string local_name;

    ext::string_vector observed_attributes;
    ext::vector<nodes::element*> construction_stack;
    html_element_constructor_t constructor;

    ext::map<ext::string, lifecycle_callback_t*> lifecycle_callbacks;

    custom_element_definition()
    {
        lifecycle_callbacks =
                {
                        {"connectedCallback", nullptr},
                        {"disconnectedCallback", nullptr},
                        {"adoptedCallback", nullptr},
                        {"attributeChangedCallback", nullptr},
                        {"formAssociatedCallback", nullptr},
                        {"formDisabledCallback", nullptr},
                        {"formResetCallback", nullptr},
                        {"formStateRestoreCallback", nullptr},
                };
    }
};


#endif //SBROWSER_CUSTOM_ELEMENTS_HPP
