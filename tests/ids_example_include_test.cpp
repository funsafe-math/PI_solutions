extern "C" {
#define TEST 1
#include "src/ids_template.c"
}
#include "tester/tester.h"

test t("#include <string.h>\n\n#define IN_LINE_COMMENT 1\n#define "
       "IN_LINE_COMMENT2 2",
       "2");