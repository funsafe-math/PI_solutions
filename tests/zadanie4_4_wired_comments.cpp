extern "C" {
#define TEST 1
#include "src/str_template.c"
}
#include "tester/tester.h"

test t("4\n/*/\n/**//", "Wpisz nr zadania 1 0");