#pragma once
#ifndef SBROWSER_XPATH_EXPRESSION_HPP
#define SBROWSER_XPATH_EXPRESSION_HPP

#include <ext/macros/decorators.hpp>

#include <dom_object.hpp>

namespace dom {
    namespace nodes {class node;}
    namespace xpath {
        class xpath_expression;
        class xpath_result;
    }
}


class dom::xpath::xpath_expression : public dom_object {
public: constructors
    xpath_expression();

public: methods
    xpath_result* evaluate(nodes::node* context_node, unsigned short type = 0, xpath_result* result = nullptr);

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;
};


#endif //SBROWSER_XPATH_EXPRESSION_HPP
