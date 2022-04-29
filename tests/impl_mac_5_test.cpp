extern "C" {
#define TEST 0
#include "src/impl_mac_template.c"
}
#include "tester/tester.h"

test t(R"(
2
2
../test_files/case_8_2.txt
)",
       "45.000000 7.200000 58.000000");
