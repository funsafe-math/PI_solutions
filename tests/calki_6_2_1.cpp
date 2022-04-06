extern "C" {
#define TEST 0
#include "src/calki_tmpl.c"
}
#include "tester/tester.h"

test t(R"(2 5.37 12.5 2000)",
       "63.72 63.71 63.71 299.83 299.71 299.71 0.84 0.84 0.84 6.70 6.69 6.69");
