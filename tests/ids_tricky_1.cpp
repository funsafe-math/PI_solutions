extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t(R"(
int main() { // comment
    printf ("Hello\n"); /* and another */
    printf ("No\n"); // joe
    return 0;
}
)",
       "2");