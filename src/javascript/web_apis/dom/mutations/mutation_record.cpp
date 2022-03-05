#include "mutation_record.hpp"


auto dom::mutations::mutation_record::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<mutation_record>{isolate}
            .inherit<dom_object>()
            .var("type", &mutation_record::type)
            .var("target", &mutation_record::target)
            .var("addedNodes", &mutation_record::added_nodes)
            .var("removedNodes", &mutation_record::removed_nodes)
            .var("previousSibling", &mutation_record::previous_sibling)
            .var("nextSibling", &mutation_record::next_sibling)
            .var("attributeName", &mutation_record::attribute_name)
            .var("attributeNamespace", &mutation_record::attribute_namespace)
            .var("oldValue", &mutation_record::old_value)
            .auto_wrap_objects();
}
