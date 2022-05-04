extern "C" {
#define TEST 0
#include "src/sorting_templ.c"
}
#include "tester/tester.h"

test t("4\n", "Anna Stone 15 Katarzyna Michalak 20 Kamila Munk 25 Anastazja "
              "Kamiska 28 Małgorzata Kowal 33 Ernest Goja 27 Emil Kant 22 "
              "Stefan Kowal 36 Kamil Mist 29 Tomasz Nod 18 Karol Wodecki 27");
