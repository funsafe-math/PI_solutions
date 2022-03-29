extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t(R"(
print"lol"
print"\""
print"// " main()
print" twoja()stara
"print/ twoja    stara  /*

1
2
3
8

\
*/
for(int i; i<0; i++);
)",
       "5");