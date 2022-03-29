extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t("/*/ test test */\nabc\n", "1");