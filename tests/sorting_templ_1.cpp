extern "C" {
#define TEST 0
#include "src/sorting_templ.c"
}
#include "tester/tester.h"

test t("1\n", "-31 0 1 2 2 4 65 83 99 782");
