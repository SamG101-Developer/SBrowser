#include "processing_instruction.hpp"


dom::nodes::processing_instruction::processing_instruction()
        : character_data()
        , css::cssom::mixins::link_style() {

    node_type = PROCESSING_INSTRUCTION_NODE;
    node_name = target;
}
