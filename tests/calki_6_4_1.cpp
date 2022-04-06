extern "C" {
#define TEST 0
#include "src/calki_tmpl.c"
}
#include "tester/tester.h"

test t(R"(4 3.6 7.2 220 4.9 8.9 200 7.2 9.4 200)", "2139.49 788.67");
