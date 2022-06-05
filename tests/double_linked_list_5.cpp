extern "C" {
#define TEST 0
#include "src/double_linked_array_list_templ.c"
}
#include "tester/tester.h"

test t(R"(
5 
)",
       "-> In, It -> Lorem, Lorem -> a, a, a, a, a, a, allows, also, and, as, "
       "available -> be, before -> called, commonly, consider, content, copy "
       "-> demonstrate, design, design, designers, document -> final, form, "
       "form -> graphic, greeking -> in, influencing, ipsum, ipsum, is, is, is "
       "-> may, meaning, meaningful -> of, of, of, on, or, or -> placeholder, "
       "placeholder, process, publication, publishing -> relying, replace -> "
       "temporarily, text, text, text, the, the, the, the, the, to, to, to, "
       "typeface -> used, used, used -> visual -> webpage, which, without, "
       "without");
