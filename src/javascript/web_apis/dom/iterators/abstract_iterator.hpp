#pragma once
#ifndef SBROWSER_ABSTRACT_ITERATOR_HPP
#define SBROWSER_ABSTRACT_ITERATOR_HPP

#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>

namespace dom {
    namespace helpers {struct traversal;}
    namespace iterators {
        class abstract_iterator;
        struct node_filter;
    }
    namespace nodes {class node;}
}


class dom::iterators::abstract_iterator {
friends
    friend struct dom::helpers::traversal;

public: constructors
    abstract_iterator();

public: properties
    ext::dom_property<nodes::node*> root;
    ext::dom_property<node_filter*> filter;
    ext::dom_property<unsigned long> what_to_show;

private:
    bool m_active_flag;
};


#endif //SBROWSER_ABSTRACT_ITERATOR_HPP
