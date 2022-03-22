#pragma once
#ifndef SBROWSER_ATTRIBUTES_HPP
#define SBROWSER_ATTRIBUTES_HPP

#include <ext/string.hpp>

namespace dom::helpers {struct attributes;}
namespace dom::nodes {class attr;}
namespace dom::nodes {class element;}


struct dom::helpers::attributes final
{
    // handle changes
    static auto handle_attributes_changes(
            const nodes::attr* attribute,
            const nodes::element* owner_element,
            const ext::string& old_value,
            const ext::string& new_value) -> void;

    // general mutations to attributes
    static auto change(
            nodes::attr* attribute,
            const ext::string& new_value) -> void;

    static auto append(
            nodes::attr* attribute,
            nodes::element* new_owner_element) -> void;

    static auto remove(
            nodes::attr* attribute) -> void;

    static auto replace(
            nodes::attr* old_attribute,
            nodes::attr* new_attribute) -> void;

    // get attribute
    static auto get_attribute_value(
            const nodes::element* owner_element,
            const ext::string& local_name,
            const ext::string& namespace_) -> ext::string;

    static auto get_attribute_by_name(
            const nodes::element* owner_element,
            const ext::string& qualified_name) -> nodes::attr*;

    static auto get_attribute_by_ns(
            const nodes::element* owner_element,
            const ext::string& local_name,
            const ext::string& namespace_) -> nodes::attr*;

    // set attribute

    static auto set_attribute_by_name(
            nodes::element* owner_element,
            const ext::string& qualified_name,
            const ext::string& value) -> nodes::attr*;

    static auto set_attribute_by_ns(
            nodes::element* owner_element,
            const ext::string& qualified_name,
            const ext::string& namespace_,
            const ext::string& value) -> nodes::attr*;

    static auto set_attribute(
            nodes::element* new_owner_element,
            nodes::attr* attribute) -> nodes::attr*;

    // remove attribute
    static auto remove_attribute_by_name(
            nodes::element* owner_element,
            const ext::string& qualified_name) -> nodes::attr*;

    static auto remove_attribute_by_ns(
            nodes::element* owner_element,
            const ext::string& local_name,
            const ext::string& namespace_) -> nodes::attr*;

    static auto remove_attribute(
            nodes::element* owner_element,
            nodes::attr* attribute) -> nodes::attr*;

    // toggle attribute
    static auto toggle_attribute_by_name(
            const nodes::element* owner_element,
            const ext::string& qualified_name,
            bool force) -> nodes::attr*;

    static auto toggle_attribute_by_ns(
            const nodes::element* owner_element,
            const ext::string& local_name,
            const ext::string& namespace_, bool force) -> nodes::attr*;

    static auto toggle_attribute(
            nodes::attr* attribute,
            bool force) -> nodes::attr*;

    // setter helpers
    static auto set_attribute_value(
            nodes::element* owner_element,
            const ext::string& local_name,
            const ext::string& value,
            const ext::string& prefix = "",
            const ext::string& namespace_ = "") -> nodes::attr*;

    static auto set_existing_attribute_value(
            nodes::attr* attribute,
            const ext::string& value) -> void;
};


#endif //SBROWSER_ATTRIBUTES_HPP
