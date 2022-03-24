extern "C" {
#define TEST 1
#include "src/str_template.c"
}
#include "tester/tester.h"

test t("2\n1\nint main() {\nprintf (\"Hello\\n\");\nreturn 0;\n}",
       "Wpisz nr zadania Wpisz numer znaku n 5");