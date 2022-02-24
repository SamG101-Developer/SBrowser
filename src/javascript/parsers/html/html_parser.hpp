#pragma once
#ifndef SBROWSER_HTML_PARSER_HPP
#define SBROWSER_HTML_PARSER_HPP

#include <ext/iterables/string.hpp>

namespace dom::nodes {class document;}
namespace javascript::parsers::html {class html_parser;}


class javascript::parsers::html::html_parser {
public:
    html_parser(ext::cstring& raw_html);
    auto parse() const -> dom::nodes::document* ;

protected:
    ext::string m_raw_html;
};


#endif //SBROWSER_HTML_PARSER_HPP
