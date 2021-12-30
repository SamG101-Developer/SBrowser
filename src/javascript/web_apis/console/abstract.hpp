#pragma once
#ifndef SBROWSER_ABSTRACT_HPP
#define SBROWSER_ABSTRACT_HPP

#include <ext/any.hpp>

namespace console {struct abstract;}


struct console::abstract {
    static void logger(ext::string log_level, ext::cvector<ext::string>& args);
    static void printer(ext::string log_level, ext::cvector<ext::any>& args, ext::cvector<ext::string>& options = {});
    static ext::string formatter(ext::vector<ext::string>&& args);
};


#endif //SBROWSER_ABSTRACT_HPP
