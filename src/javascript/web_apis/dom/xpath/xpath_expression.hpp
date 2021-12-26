#pragma once
#ifndef SBROWSER_XPATH_EXPRESSION_HPP
#define SBROWSER_XPATH_EXPRESSION_HPP

#include <ext/decorators.hpp>

namespace dom {
    namespace nodes {class node;}
    namespace xpath {
        class xpath_expression;
        class xpath_result;
    }
}


class dom::xpath::xpath_expression {
public: constructors
    xpath_expression();

public: methods
    xpath_result* evaluate(nodes::node* context_node, unsigned short type = 0, xpath_result* result = nullptr);
};


#endif //SBROWSER_XPATH_EXPRESSION_HPP
