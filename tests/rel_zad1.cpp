extern "C" {
#define TEST 0
#include "src/rel_template.c"
}
#include "tester/tester.h"

test t(R"(
1 10
2 3
2 4
3 4
0 1
0 2
0 3
0 4
1 3
1 2
1 4)",
       "0 1 0 1 1 1");
