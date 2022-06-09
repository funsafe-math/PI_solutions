extern "C" {
#define TEST 0
#include "src/ht_template.c"
}
#include "tester/tester.h"

test t(R"( 3 operating
C is the most commonly used programming Language for writing operating systems
The first operating system written in C is Unix.
Later Operating systems like GNU/Linux were all written in C.
Not only is C the Language of operating systems,it is the precursor and inspiration for almost all of the most popular
high-level Languages available today.
In fact, Perl, PHP, Python and Ruby are all written in C.
By way of analogy, let's say that you were going to be learning Spanish,
Italian, French, or Portuguese.
Do you think knowing Latin would be helpful?
Just as Latin was the basis of all of those Languages,
knowing C will enable you to understand and appreciate an entire family of programming languages
)",
       R"( 32
operating 4)");
