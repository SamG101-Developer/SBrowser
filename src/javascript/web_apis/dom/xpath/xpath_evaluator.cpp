#include "xpath_evaluator.hpp"


dom::xpath::xpath_evaluator::xpath_evaluator() : dom_object()
{
}


ext::any
dom::xpath::xpath_evaluator::v8(v8::Isolate* isolate) const
{
    return v8pp::class_<xpath_evaluator>{isolate}
            .template ctor<>()
            .template inherit<dom_object>()

            .template function("createExpression", &xpath_evaluator::create_expression)
            .template function("createNodeResolver", &xpath_evaluator::create_expression)
            .template function("evaluate", &xpath_evaluator::evaluate)

            .auto_wrap_objects();
}
