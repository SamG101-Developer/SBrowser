#pragma once
#ifndef SBROWSER_XPATH_EVALUATOR_HPP
#define SBROWSER_XPATH_EVALUATOR_HPP

#include <ext/iterables/string.hpp>
#include <ext/macros/decorators.hpp>

namespace dom {
    namespace nodes {class node;}
    namespace xpath {
        class xpath_evaluator;
        class xpath_expression;
        class xpath_result;
    }
}


class dom::xpath::xpath_evaluator {
    struct xpath_ns_resolver {ext::string lookup_namespace_uri(ext::cstring& prefix = "");};

public: constructors
    xpath_evaluator();

public: methods
    xpath_expression* create_expression(ext::cstring& expression, xpath_ns_resolver* resolver = nullptr);
    xpath_ns_resolver* create_ns_resolver(nodes::node* node_resolver);
    xpath_result evaluate(ext::cstring& expression, nodes::node* context_node, xpath_ns_resolver* resolver = nullptr, unsigned short type = 0, xpath_result* result = nullptr);
};


#endif //SBROWSER_XPATH_EVALUATOR_HPP
