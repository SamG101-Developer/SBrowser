#pragma once
#ifndef SBROWSER_COMMENT_HPP
#define SBROWSER_COMMENT_HPP

#include <dom/nodes/character_data.hpp>

namespace dom::nodes {class comment;}


class dom::nodes::comment : public character_data
{
public: constructors
    comment(const ext::string& new_data = "");

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_COMMENT_HPP
