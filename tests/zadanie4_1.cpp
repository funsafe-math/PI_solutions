extern "C" {
#define TEST 1
#include "src/str_template.c"
}
#include "tester/tester.h"

test t("1\nint main() {\n\tprintf (\"Hello\\n\");\n\treturn 0;\n}",
       "Wpisz nr zadania 4 8 47");