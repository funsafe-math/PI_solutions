extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t(R"(
int main() { // comment
    printf/* */("\"lol"); /* and // /*another */
    printf/* */("\""); // joe
    return 0;
}
)",
       "2");