extern "C" {
#define TEST 0
#include "src/vect_template.c"
}
#include "tester/tester.h"

test t(R"(2
10
p a
p X
p k
p i
p y
p R
i 1 t
i 3 G
i 5 E
d)",
       R"(16
t X G k R)");
