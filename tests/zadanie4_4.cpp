extern "C" {
#define TEST 1
#include "src/str_template.c"
}
#include "tester/tester.h"

test t("4\nint main() { // comment\nprintf (\"Hello\\n\"); /* and another "
       "*/\nreturn 0;\n/* and more\nand more ...\n*/\n}",
       "Wpisz nr zadania 2 1");