#pragma once
#ifndef SBROWSER_XPATH_EXPRESSION_HPP
#define SBROWSER_XPATH_EXPRESSION_HPP

#include <ext/decorators.hpp>

#include <dom_object.hpp>

namespace dom
{
    namespace nodes {class node;}
    namespace xpath
    {
        class xpath_expression;
        class xpath_result;
    }
}


class dom::xpath::xpath_expression : public virtual dom_object
{
public: constructors
    xpath_expression();

public: js_methods
    auto evaluate(nodes::node* context_node, ushort type = 0, xpath_result* result = nullptr) -> xpath_result*;

public: cpp_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_XPATH_EXPRESSION_HPP
