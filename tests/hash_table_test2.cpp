extern "C" {
#define TEST 0
#include "src/ht_template.c"
}
#include "tester/tester.h"

test t(R"(   1
13 3
i 12345
i 1334
i 1434
i 1534
i 3234
i 5234
i 5234
i 7234
i 8234
i 9234
i 5234
i 234
r 9234
)",
       R"(4
8234 7234 5234)");
