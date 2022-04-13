extern "C" {
#define TEST 0
#include "src/rel_template.c"
}
#include "tester/tester.h"

test t(R"(
3
7
1 2
2 3
3 4
3 2
2 5
1 5
2 4
6
2 4
1 3
5 4
3 5
3 1
1 2
)",
       "5");
