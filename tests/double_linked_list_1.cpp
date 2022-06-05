extern "C" {
#define TEST 0
#include "src/double_linked_array_list_templ.c"
}
#include "tester/tester.h"

test t(R"(
1 3 3 6 7 9 2 4 8 5 9 7 3 5 2
)",
       "-> 6, 7, 9 -> 4, 8 -> 9, 7, 3, 5, 2");
