#define TESTING 0

#if (TESTING > 0)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#else

#include <iostream>
#include "test/test_v8.hpp"
#include "test/test_qt.hpp"

#include <javascript/interop/manual_conversions.hpp>


int main(int argc, char** argv) {

    return 0;
}

#endif
