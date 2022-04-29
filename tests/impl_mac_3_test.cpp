extern "C" {
#define TEST 1
#include "src/impl_mac_template.c"
}
#include "tester/tester.h"

test t(R"(3
2
To jest wiersz 1,
a to drugi.
Trzeciego nie ma.
)",
       "Wpisz nr zadania Wpisz numer wiersza (liczony od 1) Wpisz kolejne "
       "linie zawierajace znaki ASCII, zakoncz znakiem EOF a to drugi.");
