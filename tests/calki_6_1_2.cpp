extern "C" {
#define TEST 0
#include "src/calki_tmpl.c"
}
#include "tester/tester.h"

test t(R"(1 12 15.5 4000)", "332.62");
