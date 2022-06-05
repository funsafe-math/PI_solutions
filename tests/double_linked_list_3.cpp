extern "C" {
#define TEST 0
#include "src/double_linked_array_list_templ.c"
}
#include "tester/tester.h"

test t(R"(
3 3 3 6 7 9 2 4 8 5 9 7 3 5 2 3 5 4 10
)",
       "9 7 6");
