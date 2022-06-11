extern "C" {
#define TEST 0
#include "src/ht_template.c"
}
#include "tester/tester.h"

test t(R"(  1
10 3
i 12345
i 1334
i 1534
i 3234
i 5234
i 7234
i 8234
i 9234
r 5234
i 234
)",
       R"(4
9234 8234 7234)");
