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
    text(ext::cstring& new_data = "");

public methods:
    auto split_text(unsigned long offset) -> text*;

public properties:
    ext::dom_property<ext::string, _F> whole_text;

public internal_methods:
    auto render() const -> QLabel* override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto get_whole_text() const -> ext::string;
    auto set_data(ext::cstring& val) -> void;
};


#endif //SBROWSER_TEXT_HPP
