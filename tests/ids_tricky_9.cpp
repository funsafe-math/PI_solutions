extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t(R"(
#include <string.h>
#define IN_LINE_COMMENT 1
#define IN_LINE_COMMENT2 2
)",
       "2");