#pragma once
#ifndef SBROWSER_MUTATION_RECORD_HPP
#define SBROWSER_MUTATION_RECORD_HPP

#include <ext/string.hpp>
#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <dom_object.hpp>

namespace dom
{
    namespace mutations {class mutation_record;}
    namespace nodes {class node;}
}


class dom::mutations::mutation_record : public virtual dom_object
{
public constructors:
    mutation_record();

public properties:
    ext::dom_property<ext::string> type;
    ext::dom_property<ext::string> attribute_name;
    ext::dom_property<ext::string> attribute_namespace;
    ext::dom_property<ext::string> old_value;

    ext::dom_property<nodes::node*> target;
    ext::dom_property<nodes::node*> previous_sibling;
    ext::dom_property<nodes::node*> next_sibling;

    ext::dom_property<ext::vector<nodes::node*>> added_nodes;
    ext::dom_property<ext::vector<nodes::node*>> removed_nodes;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_MUTATION_RECORD_HPP
