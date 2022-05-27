extern "C" {
#define TEST 0
#include "src/zadanie11_1_template.c"
}
#include "tester/tester.h"

test t(R"(  1
10
b 10
f 5
i 5
i 7
b 20
i 30
r
d
r
i 1
)",
       "1 5 7 10 20");
