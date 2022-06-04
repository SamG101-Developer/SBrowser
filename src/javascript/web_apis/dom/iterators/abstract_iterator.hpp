#pragma once
#ifndef SBROWSER_ABSTRACT_ITERATOR_HPP
#define SBROWSER_ABSTRACT_ITERATOR_HPP

#include <dom_object.hpp>

namespace dom::iterators {class abstract_iterator;}

namespace dom::helpers {struct traversal;}
namespace dom::iterators {struct node_filter;}
namespace dom::nodes {class node;}


class dom::iterators::abstract_iterator : public virtual dom_object
{
public friends:
    friend struct dom::helpers::traversal;

public constructors:
    abstract_iterator();

public js_properties:
    ext::property<smart_pointer<node_filter>> filter;
    ext::property<smart_pointer<nodes::node>> root;
    ext::property<ulong> what_to_show;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private:
    bool m_active_flag = false;
};


#endif //SBROWSER_ABSTRACT_ITERATOR_HPP
