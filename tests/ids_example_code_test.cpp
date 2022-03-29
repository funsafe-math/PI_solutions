extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t("int main() { // comment\nprintf (\"Hello\\n\"); /* and another "
       "*/\nprintf (\"Greetings\\n\");\nreturn 0;\n}",
       "2");