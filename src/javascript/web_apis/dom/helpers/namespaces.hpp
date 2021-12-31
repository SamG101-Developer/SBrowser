#pragma once
#ifndef SBROWSER_NAMESPACES_HPP
#define SBROWSER_NAMESPACES_HPP

#include <ext/types/typedefs.hpp>

namespace dom::helpers {struct namespaces;}


struct dom::helpers::namespaces {
    struct_string HTML = "http://www.w3.org/1999/xhtml";
    struct_string MATHML = "http://www.w3.org/1998/Math/MathML";
    struct_string SVG = "http://www.w3.org/2000/svg";
    struct_string XLINK = "http://www.w3.org/1999/xlink";
    struct_string XML = "http://www.w3.org/XML/1998/namespace";
    struct_string XMLNS = "http://www.w3.org/2000/xmlns/";

    static void validate(ext::cstring& qualified_name);
    static std::tuple<ext::string, ext::string, ext::string> validate_and_extract(ext::cstring& namespace_, ext::cstring& qualified_name);
};


#endif //SBROWSER_NAMESPACES_HPP
