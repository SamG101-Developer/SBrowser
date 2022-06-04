#pragma once
#ifndef SBROWSER_MUTATION_RECORD_HPP
#define SBROWSER_MUTATION_RECORD_HPP

#include <dom_object.hpp>
namespace dom::mutations {class mutation_record;}

#include <ext/string.hpp>
namespace dom::nodes {class node;}


class dom::mutations::mutation_record : public virtual dom_object
{
public constructors:
    mutation_record();

public js_properties:
    ext::property<ext::string> type;
    ext::property<ext::string> attribute_name;
    ext::property<ext::string> attribute_namespace;
    ext::property<ext::string> old_value;

    ext::property<smart_pointer<nodes::node>> target;
    ext::property<smart_pointer<nodes::node>> previous_sibling;
    ext::property<smart_pointer<nodes::node>> next_sibling;

    ext::property<smart_pointer<ext::vector<smart_pointer<nodes::node>>>> added_nodes;
    ext::property<smart_pointer<ext::vector<smart_pointer<nodes::node>>>> removed_nodes;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_MUTATION_RECORD_HPP
