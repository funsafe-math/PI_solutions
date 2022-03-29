extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t(R"(
#include <something> lol
for(int i; i<0; i++);
char hej

)",
       "2");