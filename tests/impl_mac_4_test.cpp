extern "C" {
#define TEST 1
#include "src/impl_mac_template.c"
}
#include "tester/tester.h"

test t(R"(4
2
1 2 3 4 5
-1.0 2
8
12 3 1
)",
       "Wpisz nr zadania Wpisz numer wiersza (liczony od 1) w kolejnosci "
       "rosnacej sredniej Wpisz kolejne linie zawierajace liczby, zakoncz "
       "znakiem EOF 1.000000 2.000000 3.000000 4.000000 5.000000 3.000000");
