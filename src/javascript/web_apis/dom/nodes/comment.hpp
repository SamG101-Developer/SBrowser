#pragma once
#ifndef SBROWSER_COMMENT_HPP
#define SBROWSER_COMMENT_HPP

#include <dom/nodes/character_data.hpp>

namespace dom::nodes {class comment;}


class dom::nodes::comment : public dom::nodes::character_data {
public constructors:
    comment(ext::cstring& new_data = "");
};


#endif //SBROWSER_COMMENT_HPP
