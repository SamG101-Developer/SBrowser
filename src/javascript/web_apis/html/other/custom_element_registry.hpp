#pragma once
#ifndef SBROWSER_CUSTOM_ELEMENT_REGISTRY_HPP
#define SBROWSER_CUSTOM_ELEMENT_REGISTRY_HPP

#include <optional>

#include <dom_object.hpp>
#include <dom/helpers/custom_elements.hpp>
#include <web_idl/types/promise.hpp>

namespace dom::internal {struct custom_element_definition;}
namespace html::other {class custom_element_registry;}


class html::other::custom_element_registry : public dom_object
{
public aliases:
    using html_element_constructor_t = dom::internal::custom_element_definition::html_element_constructor_t;
    
public js_methods:
    auto define(const ext::string& name, const html_element_constructor_t& constructor, const ext::string_any_map& options = {}) -> void;
    auto get(const ext::string name) -> std::optional<html_element_constructor_t>;
    auto when_defined(const ext::string name) -> webidl::types::promise<html_element_constructor_t>;
    auto upgrade(dom::nodes::node* root) -> void;

private cpp_properties:
    ext::vector<dom::internal::custom_element_definition*> m_custom_element_definitions;
    ext::map<ext::string, webidl::types::promise<html_element_constructor_t>> m_when_defined_promise_map;
    bool m_is_element_definition_running_flag;
};


#endif //SBROWSER_CUSTOM_ELEMENT_REGISTRY_HPP
