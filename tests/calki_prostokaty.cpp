extern "C" {
#define TEST 0
#include "src/calki_tmpl.c"
}
#include "tester/tester.h"

test t(R"(1 0 10 1000)", "166.42");
