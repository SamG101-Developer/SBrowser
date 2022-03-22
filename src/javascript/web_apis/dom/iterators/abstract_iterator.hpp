#pragma once
#ifndef SBROWSER_ABSTRACT_ITERATOR_HPP
#define SBROWSER_ABSTRACT_ITERATOR_HPP

#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <dom_object.hpp>

namespace dom::helpers {struct traversal;}
namespace dom::iterators {class abstract_iterator;}
namespace dom::iterators {struct node_filter;}
namespace dom::nodes {class node;}


class dom::iterators::abstract_iterator : virtual public dom_object
{
public friends:
    friend struct dom::helpers::traversal;

public constructors:
    abstract_iterator();

public properties:
    ext::dom_property<nodes::node*, _F> root;
    ext::dom_property<node_filter*, _F> filter;
    ext::dom_property<unsigned long, _F> what_to_show;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private:
    bool m_active_flag = false;
};


#endif //SBROWSER_ABSTRACT_ITERATOR_HPP
