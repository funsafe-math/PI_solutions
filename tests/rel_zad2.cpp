extern "C" {
#define TEST 0
#include "src/rel_template.c"
}
#include "tester/tester.h"

test t(R"(
2
12
1 4
1 1
1 5
1 6
2 4
2 2
2 6
3 4
3 3
4 4
6 6
5 5
)",
       R"(1 0
6
1 2 3 4 5 6
3
4 5 6
3
1 2 3
)");
