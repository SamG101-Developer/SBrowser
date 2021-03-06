#include "xpath_expression.hpp"


dom::xpath::xpath_expression::xpath_expression() : dom_object()
{
}


auto dom::xpath::xpath_expression::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<xpath_expression>{isolate}
            .inherit<dom_object>()
            .function("evaluate", &xpath_expression::evaluate)
            .auto_wrap_objects();
}
