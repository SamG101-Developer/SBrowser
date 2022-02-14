#pragma once
#ifndef SBROWSER_XPATH_RESULT_HPP
#define SBROWSER_XPATH_RESULT_HPP

#include <ext/macros/decorators.hpp>
#include <ext/properties/dom_property.hpp>

#include <dom_object.hpp>

namespace dom {
    namespace nodes {class node;}
    namespace xpath {class xpath_result;}
}


class dom::xpath::xpath_result : public dom_object {
public: constructors
    xpath_result();

public: static_constants
    static const unsigned short ANY_TYPE = 0;
    static const unsigned short NUMBER_TYPE = 1;
    static const unsigned short STRING_TYPE = 2;
    static const unsigned short BOOLEAN_TYPE = 3;
    static const unsigned short UNORDERED_NODE_ITERATOR_TYPE = 4;
    static const unsigned short ORDERED_NODE_ITERATOR_TYPE = 5;
    static const unsigned short UNORDERED_NODE_SNAPSHOT_TYPE = 6;
    static const unsigned short ORDERED_NODE_SNAPSHOT_TYPE = 7;
    static const unsigned short ANY_UNORDERED_NODE_TYPE = 8;
    static const unsigned short FIRST_ORDERED_NODE_TYPE = 9;

public: methods
    nodes::node* iterate_next();
    nodes::node* snapshot_item(unsigned long index);

public: properties
    ext::dom_property<unsigned short, _F> result_type;
    ext::dom_property<double, _F> number_value;
    ext::dom_property<ext::string, _F> string_value;
    ext::dom_property<bool, _F> boolean_value;
    ext::dom_property<nodes::node*, _F> single_node_value;
    ext::dom_property<bool, _F> invalid_iterator_state;
    ext::dom_property<unsigned long, _F> snapshot_length;

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;
};


#endif //SBROWSER_XPATH_RESULT_HPP
