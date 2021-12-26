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
    ext::property<ext::string> type;
    ext::property<ext::string> attribute_name;
    ext::property<ext::string> attribute_namespace;
    ext::property<ext::string> old_value;

    ext::property<nodes::node*> target;
    ext::property<nodes::node*> previous_sibling;
    ext::property<nodes::node*> next_sibling;

    ext::property<ext::vector<nodes::node*>> added_nodes;
    ext::property<ext::vector<nodes::node*>> removed_nodes;
};


#endif //SBROWSER_MUTATION_RECORD_HPP
