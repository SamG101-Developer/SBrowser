#pragma once
#ifndef SBROWSER_TEXTS_HPP
#define SBROWSER_TEXTS_HPP

#include <ext/iterables/string.hpp>

namespace dom
{
    namespace helpers {struct texts;}
    namespace nodes
    {
        class character_data;
        class element;
        class text;
    }
}

struct dom::helpers::texts
{
    static auto replace_data(nodes::character_data* text_node, unsigned long offset, unsigned long count, const ext::string& data) -> void;
    static auto split(nodes::text* text_node, unsigned long offset) -> nodes::text*;
    static auto substring_data(const nodes::character_data* text_node, unsigned long offset, unsigned long count) -> ext::string;
    static auto advisory_information(nodes::element* element) -> ext::string;
};


#endif //SBROWSER_TEXTS_HPP
