#include "mutation_record.hpp"


dom::mutations::mutation_record::mutation_record() = default;


auto dom::mutations::mutation_record::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<mutation_record>{isolate}
            .template inherit<dom_object>()
            .template var("type", &mutation_record::type)
            .template var("target", &mutation_record::target)
            .template var("addedNodes", &mutation_record::added_nodes)
            .template var("removedNodes", &mutation_record::removed_nodes)
            .template var("previousSibling", &mutation_record::previous_sibling)
            .template var("nextSibling", &mutation_record::next_sibling)
            .template var("attributeName", &mutation_record::attribute_name)
            .template var("attributeNamespace", &mutation_record::attribute_namespace)
            .template var("oldValue", &mutation_record::old_value)
            .auto_wrap_objects();
}
