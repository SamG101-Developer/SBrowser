#pragma once
#ifndef SBROWSER_TEXTS_HPP
#define SBROWSER_TEXTS_HPP

#include <ext/string.hpp>

namespace dom::helpers {struct texts;}
namespace dom::nodes {class character_data;}
namespace dom::nodes {class element;}
namespace dom::nodes {class text;}


struct dom::helpers::texts final
{
    // manipulate text
    static auto replace_data(
            nodes::character_data* text_node,
            unsigned long offset,
            unsigned long count,
            const ext::string& data) -> void;

    static auto split(
            nodes::text* text_node,
            unsigned long offset) -> nodes::text*;

    static auto substring_data(
            const nodes::character_data* text_node,
            unsigned long offset,
            unsigned long count) -> ext::string;

    // other general helper methods for text
    static auto advisory_information(
            nodes::element* element) -> ext::string;
};


#endif //SBROWSER_TEXTS_HPP
