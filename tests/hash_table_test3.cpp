extern "C" {
#define TEST 0
#include "src/ht_template.c"
}
#include "tester/tester.h"

test t(R"(2
12 2
i C
i i
i t
i M
r C
i u
i P
i l
i f
i W
r o
i s
)",
       R"(4
l M t)");
