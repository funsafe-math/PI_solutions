extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t(R"(
int main() {
    for (int i=0; i<5; i++)
    {
        printf("Joe mama");
    }
    return 0;
    i = 5;
}
)",
       "3");