extern "C" {
#define TEST 0
#include "src/vect_template.c"
}
#include "tester/tester.h"

test t(R"(3
8
p 23 Dominik Adamczyk
p 27 Natalia Adamiak
p 24 Marcin Chudy
i 1 29 Anna Cichocka
i 4 22 Natalia Deyna
i 0 24 Marcin Bereta
d
s)",
       R"(8
24 Marcin Bereta
24 Marcin Chudy
23 Dominik Adamczyk
22 Natalia Deyna)");
