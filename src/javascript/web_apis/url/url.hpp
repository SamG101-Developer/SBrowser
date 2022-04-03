#pragma once
#ifndef SBROWSER_URL_HPP
#define SBROWSER_URL_HPP

#include <ext/string.hpp>

namespace url {class url;}


class url::url
{
public cpp_methods:
    auto empty() -> bool;

public operators:
    auto operator=(const ext::string& val) -> void;
    auto operator=(const char* val) -> void;
};


#endif //SBROWSER_URL_HPP
