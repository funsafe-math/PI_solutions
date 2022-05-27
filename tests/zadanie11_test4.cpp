extern "C" {
#define TEST 0
#include "src/zadanie11_1_template.c"
}
#include "tester/tester.h"

test t(R"( 4
3
../test_files/Tekst_C.txt
)",
       "and languages systems written you");
