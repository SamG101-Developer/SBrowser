#pragma once
#ifndef SBROWSER_XPATH_EVALUATOR_HPP
#define SBROWSER_XPATH_EVALUATOR_HPP

#include <ext/string.hpp>
#include <ext/decorators.hpp>

#include <dom_object.hpp>

namespace dom
{
    namespace nodes {class node;}
    namespace xpath
    {
        class xpath_evaluator;
        class xpath_expression;
        class xpath_result;
    }
}


class dom::xpath::xpath_evaluator : public virtual dom_object
{
    struct xpath_ns_resolver {ext::string lookup_namespace_uri(const ext::string& prefix = "");};

public: constructors
    xpath_evaluator();

public: js_methods
    auto create_expression(const ext::string& expression, xpath_ns_resolver* resolver = nullptr) -> xpath_expression*;
    auto create_ns_resolver(nodes::node* node_resolver) -> xpath_ns_resolver*;
    auto evaluate(const ext::string& expression, nodes::node* context_node, xpath_ns_resolver* resolver = nullptr, ushort type = 0, xpath_result* result = nullptr) -> xpath_result;

public: cpp_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_XPATH_EVALUATOR_HPP
