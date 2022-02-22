#ifndef SBROWSER_HELPERS_HPP
#define SBROWSER_HELPERS_HPP

#include <v8.h>

namespace javascript::helpers {
    static auto new_target() -> v8::Object;
    static auto active_function_object() -> v8::Object;
}

#endif //SBROWSER_HELPERS_HPP
