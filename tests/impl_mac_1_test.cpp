extern "C" {
#define TEST 0
#include "src/impl_mac_template.c"
}
#include "tester/tester.h"

test t(R"(1
2 3
1 2 3
4 5 6
3 2
10 20
30 40
50 60
)",
       "220.00 280.00 490.00 640.00");
