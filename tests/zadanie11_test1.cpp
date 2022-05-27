extern "C" {
#define TEST 0
#include "src/zadanie11_1_template.c"
}
#include "tester/tester.h"

test t(R"( 1
7
b 10
f 20
b 30
d
f 40
r
d)",
       "10 40");
