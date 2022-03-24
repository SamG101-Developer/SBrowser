#pragma once
#ifndef SBROWSER_NAMESPACES_HPP
#define SBROWSER_NAMESPACES_HPP

#include <ext/equalities.hpp>

namespace dom::helpers {struct namespaces;}


struct dom::helpers::namespaces final
{
    inline static constexpr const char* HTML = "http://www.w3.org/1999/xhtml";
    inline static constexpr const char* MATHML = "http://www.w3.org/1998/Math/MathML";
    inline static constexpr const char* SVG = "http://www.w3.org/2000/svg";
    inline static constexpr const char* XLINK = "http://www.w3.org/1999/xlink";
    inline static constexpr const char* XML = "http://www.w3.org/XML/1998/namespace";
    inline static constexpr const char* XMLNS = "http://www.w3.org/2000/xmlns/";
    inline static constexpr const char* NONE = "";

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
