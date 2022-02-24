#pragma once
#ifndef SBROWSER_ATTRIBUTES_HPP
#define SBROWSER_ATTRIBUTES_HPP

#include <ext/iterables/string.hpp>

namespace dom
{
    namespace helpers {struct attributes;}
    namespace nodes
    {
        class attr;
        class element;
    }
}


struct dom::helpers::attributes
{
    static auto handle_attributes_changes(const nodes::attr* attribute, const nodes::element* owner_element, ext::cstring& old_value, ext::cstring& new_value) -> void;

    static auto change(nodes::attr* attribute, ext::cstring& new_value) -> void;
    static auto append(nodes::attr* attribute, nodes::element* new_owner_element) -> void;
    static auto remove(nodes::attr* attribute) -> void;
    static auto replace(nodes::attr* old_attribute, nodes::attr* new_attribute) -> void;

    static auto get_attribute_value(const nodes::element* owner_element,  ext::cstring& local_name, ext::cstring& namespace_) -> ext::string;
    static auto get_attribute_by_name(const nodes::element* owner_element, ext::cstring& qualified_name) -> nodes::attr*;
    static auto get_attribute_by_ns(const nodes::element* owner_element, ext::cstring& local_name, ext::cstring& namespace_) -> nodes::attr*;

    static auto set_attribute_value(nodes::element* owner_element, ext::cstring& local_name, ext::cstring& value, ext::cstring& prefix = "", ext::cstring& namespace_ = "") -> nodes::attr*;
    static auto set_attribute_by_name(nodes::element* owner_element, ext::cstring& qualified_name, ext::cstring& value) -> nodes::attr*;
    static auto set_attribute_by_ns(nodes::element* owner_element, ext::cstring& qualified_name, ext::cstring& namespace_, ext::cstring& value) -> nodes::attr*;
    static auto set_attribute(nodes::element* new_owner_element, nodes::attr* attribute) -> nodes::attr*;

    static auto remove_attribute_by_name(nodes::element* owner_element, ext::cstring& qualified_name) -> nodes::attr*;
    static auto remove_attribute_by_ns(nodes::element* owner_element, ext::cstring& local_name, ext::cstring& namespace_) -> nodes::attr*;
    static auto remove_attribute(nodes::element* owner_element, nodes::attr* attribute) -> nodes::attr*;

    static auto toggle_attribute_by_name(const nodes::element* owner_element, ext::cstring& qualified_name, bool force) -> nodes::attr*;
    static auto toggle_attribute_by_ns(const nodes::element* owner_element, ext::cstring& local_name, ext::cstring& namespace_, bool force) -> nodes::attr*;
    static auto toggle_attribute(nodes::attr* attribute, bool force) -> nodes::attr*;

    static auto set_existing_attribute_value(nodes::attr* attribute, ext::cstring& value) -> void;
};


#endif //SBROWSER_ATTRIBUTES_HPP
