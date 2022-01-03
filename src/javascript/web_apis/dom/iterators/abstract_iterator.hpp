#pragma once
#ifndef SBROWSER_ABSTRACT_ITERATOR_HPP
#define SBROWSER_ABSTRACT_ITERATOR_HPP

#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>
#include <dom_object.hpp>

namespace dom {
    namespace helpers {struct traversal;}
    namespace iterators {
        class abstract_iterator;
        struct node_filter;
    }
    namespace nodes {class node;}
}


class dom::iterators::abstract_iterator : public dom_object {
friends
    friend struct dom::helpers::traversal;

public: constructors
    abstract_iterator();

public: properties
    ext::dom_property<nodes::node*> root;
    ext::dom_property<node_filter*> filter;
    ext::dom_property<unsigned long> what_to_show;

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;

private:
    bool m_active_flag;
};


#endif //SBROWSER_ABSTRACT_ITERATOR_HPP
