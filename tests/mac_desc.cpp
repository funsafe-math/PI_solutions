extern "C" {
#define TEST 0
#include "src/mac_2_template.c"
}
#include "tester/tester.h"

test t(R"(
1
4
1 -1 2 -1
2 -2 3 -3
1 1 1 0
1 -1 4 3
-8 -20 -2 4
)",
       "4.0000 -7.0000 3.0000 2.0000 2.0000");
