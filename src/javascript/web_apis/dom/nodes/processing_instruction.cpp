#include "processing_instruction.hpp"


dom::nodes::processing_instruction::processing_instruction()
        : character_data()
        , css::cssom::mixins::link_style() {

    node_name = target;
}
