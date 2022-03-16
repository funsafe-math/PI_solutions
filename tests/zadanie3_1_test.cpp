extern "C" {
#include "src/statystyka_tmpl.c"
}
#include "tester/tester.h"

test t("1 7 5 87 22 45 33 78 1",
       "średnia arith_aver = 38.71 wariancja = 969.35");