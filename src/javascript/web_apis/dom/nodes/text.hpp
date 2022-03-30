#pragma once
#ifndef SBROWSER_TEXT_HPP
#define SBROWSER_TEXT_HPP

#include <dom/mixins/slottable.hpp>
#include <dom/nodes/character_data.hpp>

#include <QtWidgets/QLabel>

namespace dom::nodes {class text;}


class dom::nodes::text
        : public character_data
        , public mixins::slottable<text>
{
public constructors:
    text(const ext::string& new_data = "");

public js_methods:
    auto split_text(unsigned long offset) -> text*;

public js_properties:
    ext::dom_property<ext::string> whole_text;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    auto get_whole_text() const -> ext::string;
    auto set_data(const ext::string& val) -> void;
};


#endif //SBROWSER_TEXT_HPP
