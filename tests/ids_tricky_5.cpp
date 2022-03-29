extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t(R"(
int main() { // comment
    printf/*lol*/("\"lol"); /* and // /* another */
    printf/* one \*/ two */("\""); // joe // mama
    return 0;
}
)",
       "2");