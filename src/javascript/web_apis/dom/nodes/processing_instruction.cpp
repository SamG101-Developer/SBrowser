#include "processing_instruction.hpp"


dom::nodes::processing_instruction::processing_instruction()
        : character_data()
        // , css::cssom::mixins::link_style()
{
    // set the properties
    node_type << PROCESSING_INSTRUCTION_NODE;
    node_name << target;
}


auto dom::nodes::processing_instruction::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<processing_instruction>{isolate}
            .template inherit<character_data>()
            .template var("target", &processing_instruction::target)
            .auto_wrap_objects();
}
