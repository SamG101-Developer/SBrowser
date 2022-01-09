#pragma once
#ifndef SBROWSER_TEXTS_HPP
#define SBROWSER_TEXTS_HPP

#include <ext/iterables/string.hpp>

namespace dom {
    namespace helpers {struct texts;}
    namespace nodes {
        class character_data;
        class element;
        class text;
    }
}

struct dom::helpers::texts {
    static void replace_data(nodes::character_data* text_node, unsigned long offset, unsigned long count, ext::cstring& data);
    static nodes::text* split(nodes::text* text_node, unsigned long offset);
    static ext::string substring_data(const nodes::character_data* text_node, unsigned long offset, unsigned long count);
    static ext::string advisory_information(nodes::element* element);
};


#endif //SBROWSER_TEXTS_HPP
