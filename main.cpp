#define TESTING 0

#if (TESTING > 0)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#else

#include <iostream>
#include "tests/test_v8.hpp"
#include "tests/test_qt.hpp"

#include <javascript/interop/expose_cpp_to_js.hpp>


int main(int argc, char** argv) {

    return 0;
}

#endif
