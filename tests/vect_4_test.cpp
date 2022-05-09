extern "C" {
#define TEST 0
#include "src/vect_template.c"
}
#include "tester/tester.h"

test t(R"(3
13
p 23 Dominik Adamczyk
p 27 Natalia Adamiak
p 25 Jan Augustyn
p 24 Kacper Augustyn
p 23 Ewelina Badeja
i 0 22 Jakub Barber
i 1 24 Kacper Barczynski
i 3 21 Dorota Bujak
e 0 22 Jakub Barber
e 1 24 Kacper Augustyn
d
f
s)",
       R"(4
25 Jan Augustyn
24 Kacper Barczynski
23 Ewelina Badeja
21 Dorota Bujak)");
