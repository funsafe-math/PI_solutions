extern "C" {
#define TEST 1
#include "src/mac_2_template.c"
}
#include "tester/tester.h"

test t(R"(
2
3
1 2 -1
2 1 0
-1 1 2
)",
       R"(-9.0000
-0.2222 0.5556 -0.1111
0.4444 -0.1111 0.2222
-0.3333 0.3333 0.3333
)");
