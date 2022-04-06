extern "C" {
#define TEST 0
#include "src/calki_tmpl.c"
}
#include "tester/tester.h"

test t(R"(4 3.2 5.2 200 3.1 4.9 200 50.1 55.2 200)", "5129.19 2037.96");
