extern "C" {
#define TEST 0
#include "src/calki_tmpl.c"
}
#include "tester/tester.h"

test t(R"(3 17.5 19.7 1500 3.4 9 1000)", "4531.55 117.12 491.00");
