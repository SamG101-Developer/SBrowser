#pragma once
#ifndef SBROWSER_MUTATION_RECORD_HPP
#define SBROWSER_MUTATION_RECORD_HPP

#include <ext/decorators.hpp>
#include <ext/property.hpp>
#include <ext/string.hpp>

namespace dom {
    namespace mutations {class mutation_record;}
    namespace nodes {class node;}
}


class dom::mutations::mutation_record {
public: properties
    ext::dom_property<ext::string> type;
    ext::dom_property<ext::string> attribute_name;
    ext::dom_property<ext::string> attribute_namespace;
    ext::dom_property<ext::string> old_value;

    ext::dom_property<nodes::node*> target;
    ext::dom_property<nodes::node*> previous_sibling;
    ext::dom_property<nodes::node*> next_sibling;

    ext::dom_property<ext::vector<nodes::node*>> added_nodes;
    ext::dom_property<ext::vector<nodes::node*>> removed_nodes;
};


#endif //SBROWSER_MUTATION_RECORD_HPP
