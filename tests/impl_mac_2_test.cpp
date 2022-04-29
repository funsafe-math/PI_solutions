extern "C" {
#define TEST 1
#include "src/impl_mac_template.c"
}
#include "tester/tester.h"

test
    t(R"(2
3
2 3. 1e-1 -5
1 2 3
0 -5
)",
      "Wpisz nr zadania Wpisz numer wiersza (liczony od 1) Wpisz kolejne linie "
      "zawierajace liczby, zakoncz znakiem EOF 1.000000 2.000000 3.000000");
