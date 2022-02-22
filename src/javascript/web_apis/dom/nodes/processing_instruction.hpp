#pragma once
#ifndef SBROWSER_PROCESSING_INSTRUCTION_HPP
#define SBROWSER_PROCESSING_INSTRUCTION_HPP

#include <dom/nodes/character_data.hpp>

namespace dom::nodes {class processing_instruction;}


class dom::nodes::processing_instruction
        : public character_data {
        // , public css::cssom::mixins::link_style {

public constructors:
    processing_instruction();

public properties:
    ext::dom_property<ext::string, _F> target;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_PROCESSING_INSTRUCTION_HPP
