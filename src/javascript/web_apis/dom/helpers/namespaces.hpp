#pragma once
#ifndef SBROWSER_NAMESPACES_HPP
#define SBROWSER_NAMESPACES_HPP

#include <ext/equalities.hpp>

namespace dom::helpers {struct namespaces;}


struct dom::helpers::namespaces final
{
    inline static const ext::string HTML = "http://www.w3.org/1999/xhtml";
    inline static const ext::string MATHML = "http://www.w3.org/1998/Math/MathML";
    inline static const ext::string SVG = "http://www.w3.org/2000/svg";
    inline static const ext::string XLINK = "http://www.w3.org/1999/xlink";
    inline static const ext::string XML = "http://www.w3.org/XML/1998/namespace";
    inline static const ext::string XMLNS = "http://www.w3.org/2000/xmlns/";
    inline static const ext::string NONE = "";

    // validation checks
    static auto validate(
            const ext::string& qualified_name)
            -> void;

    static auto validate_and_extract(
            const ext::string& namespace_,
            const ext::string& qualified_name)
            -> std::tuple<ext::string, ext::string, ext::string>;
};


#endif //SBROWSER_NAMESPACES_HPP
