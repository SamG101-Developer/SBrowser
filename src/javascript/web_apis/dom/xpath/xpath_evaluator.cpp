#include "xpath_evaluator.hpp"


dom::xpath::xpath_evaluator::xpath_evaluator() : dom_object()
{
}


ext::any
dom::xpath::xpath_evaluator::v8(v8::Isolate* isolate) const
{
    return v8pp::class_<xpath_evaluator>{isolate}
            .ctor<>()
            .inherit<dom_object>()

            .function("createExpression", &xpath_evaluator::create_expression)
            .function("createNodeResolver", &xpath_evaluator::create_expression)
            .function("evaluate", &xpath_evaluator::evaluate)

            .auto_wrap_objects();
}
