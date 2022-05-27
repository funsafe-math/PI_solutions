extern "C" {
#define TEST 0
#include "src/zadanie11_1_template.c"
}
#include "tester/tester.h"

test t(R"( 3
../test_files/Tekst_1.txt
)",
       "In fact Perl PHP Python and Ruby are all written in C");
