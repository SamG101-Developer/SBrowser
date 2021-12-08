#pragma once
#ifndef SBROWSER_TEXT_HPP
#define SBROWSER_TEXT_HPP

#include <QtWidgets/QLabel>
#include <dom/nodes/character_data.hpp>

namespace dom::nodes {class text;}


class dom::nodes::text
        : public dom::nodes::character_data
        , public dom::mixins::slottable<dom::nodes::character_data> {

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
