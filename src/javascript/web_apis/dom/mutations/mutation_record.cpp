#include "mutation_record.hpp"


dom::mutations::mutation_record::mutation_record() : dom_object()
{
}


ext::any
dom::mutations::mutation_record::v8(v8::Isolate* isolate) const
{
    return v8pp::class_<mutation_record>{isolate}
            .inherit<dom_object>()
            .var("type", &dom::mutations::mutation_record::type)
            .var("target", &dom::mutations::mutation_record::target)
            .var("addedNodes", &dom::mutations::mutation_record::added_nodes)
            .var("removedNodes", &dom::mutations::mutation_record::removed_nodes)
            .var("previousSibling", &dom::mutations::mutation_record::previous_sibling)
            .var("nextSibling", &dom::mutations::mutation_record::next_sibling)
            .var("attributeName", &dom::mutations::mutation_record::attribute_name)
            .var("attributeNamespace", &dom::mutations::mutation_record::attribute_namespace)
            .var("oldValue", &dom::mutations::mutation_record::old_value)
            .auto_wrap_objects();
}
