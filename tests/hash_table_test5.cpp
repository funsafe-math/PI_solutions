extern "C" {
#define TEST 0
#include "src/ht_template.c"
}
#include "tester/tester.h"

test t(R"( 3 xyz
abcd xyz ab;XYz qwerty
)",
       R"(8
xyz 2)");
