extern "C" {
#define TEST 0
#include "src/calki_tmpl.c"
}
#include "tester/tester.h"

test t(R"(1 5 11 1000)", "200.86");
