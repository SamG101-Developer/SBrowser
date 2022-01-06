#pragma once
#ifndef SBROWSER_TEXT_HPP
#define SBROWSER_TEXT_HPP

#include <dom/mixins/slottable.hpp>
#include <dom/nodes/character_data.hpp>

#include <QtWidgets/QLabel>

namespace dom::nodes {class text;}


class dom::nodes::text
        : public character_data
        , public mixins::slottable<text> {

public constructors:
    text(ext::cstring& new_data = "");

public methods:
    text* split_text(unsigned long offset);

public properties:
    ext::dom_property<ext::string> whole_text;

public internal_methods:
    QLabel* render() override;

private accessors:
    ext::string get_whole_text() const;
    void set_data(ext::cstring& val);
};


#endif //SBROWSER_TEXT_HPP
