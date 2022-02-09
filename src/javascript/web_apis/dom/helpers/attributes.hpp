#pragma once
#ifndef SBROWSER_ATTRIBUTES_HPP
#define SBROWSER_ATTRIBUTES_HPP

#include <ext/iterables/string.hpp>

namespace dom {
    namespace helpers {struct attributes;}
    namespace nodes {
        class attr;
        class element;
    }
}


struct dom::helpers::attributes {
    static void handle_attributes_changes(const nodes::attr* attribute, const nodes::element* owner_element, ext::cstring& old_value, ext::cstring& new_value);

    static void change(nodes::attr* attribute, ext::cstring& new_value);
    static void append(nodes::attr* attribute, nodes::element* new_owner_element);
    static void remove(nodes::attr* attribute);
    static void replace(nodes::attr* old_attribute, nodes::attr* new_attribute);

    // TODO : move owner_element parameter to be the first parameter in all of these methods
    static nodes::attr* get_attribute_by_name(ext::cstring& qualified_name, const nodes::element* owner_element);
    static nodes::attr* get_attribute_by_ns(ext::cstring& namespace_, ext::cstring& local_name, const nodes::element* owner_element);
    static ext::string get_attribute_value(const nodes::element* owner_element, ext::cstring& namespace_, ext::cstring& local_name = "");

    static nodes::attr* set_attribute(nodes::attr* attribute, nodes::element* new_owner_element);
    static nodes::attr* set_attribute_value(nodes::element* owner_element, ext::cstring& local_name, ext::cstring& value, ext::cstring& prefix = "", ext::cstring& namespace_ = "");

    static nodes::attr* remove_attribute_by_name(ext::cstring& qualified_name, const nodes::element* owner_element);
    static nodes::attr* remove_attribute_by_ns(ext::cstring& namespace_, ext::cstring& local_name, const nodes::element* owner_element);

    static void set_existing_attribute_value(nodes::attr* attribute, ext::cstring& value);
};


#endif //SBROWSER_ATTRIBUTES_HPP
