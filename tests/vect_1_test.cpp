extern "C" {
#define TEST 0
#include "src/vect_template.c"
}
#include "tester/tester.h"

test t(R"(1
12
p 10
p 20
p 5
p 3
p 15
i 0 30
i 4 40
i 7 50
e 4 10
e 4 20
r 6
f)",
       R"(6
30 5 3 50 0 0)");
