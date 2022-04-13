#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000
#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

// Create missing boolean type to improve readability
typedef int cbool;
#define true 1
#define false 0

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int int_cmp(const void *a, const void *b) {
  int arg1 = *(const int *)a;
  int arg2 = *(const int *)b;
  return arg1 - arg2;
}

// Set
typedef struct {
  int v[MAX_RANGE];
  int n;
} set_t;

set_t set_gen() {
  set_t r;
  r.n = 0;
  return r;
}

void set_sort(set_t *s) {
  qsort(s->v, s->n, sizeof(s->v[0]), int_cmp);
}

cbool set_contains(const set_t *s, int e) {
  return bsearch(&e, s->v, s->n, sizeof(s->v[0]), int_cmp) != NULL;
}

// Adds element to set, do nothing if element already in set
void set_add(set_t *s, int e) {
  if (set_contains(s, e))
    return;
  if (s->n + 1 >= (int)ARR_LEN(s->v))
    abort();
  s->v[s->n++] = e;
  set_sort(s);
}

// Pair
typedef struct {
  int first;
  int second;
} pair;

int pair_cmp_first_only(const void *first, const void *second) {
  int a = ((pair *)first)->first;
  int b = ((pair *)second)->first;
  return a - b;
}

int pair_cmp_second_only(const void *first, const void *second) {
  int a = ((pair *)first)->second;
  int b = ((pair *)second)->second;
  return a - b;
}

int pair_cmp(const void *first, const void *second) {
  pair *a = ((pair *)first);
  pair *b = ((pair *)second);
  if (a->first == b->first)
    return a->second - b->second;
  return a->first - b->first;
}

// Pair constructor
pair pair_gen(int a, int b) {
  pair r;
  // = (pair){.first = a, .second = b}; compound literals forbidden by ISO C++
  r.first = a;
  r.second = b;
  return r;
}

// Swaps first and second elements
pair pair_reverse(pair p) {
  swap(&p.first, &p.second);
  return p;
}

cbool pair_equal(pair a, pair b) {
  return pair_cmp(&a, &b) == 0;
}

// I want my classes !!!
// Relation
typedef struct {
  pair pairs[MAX_REL_SIZE];
  unsigned int n;
} relation_t;

relation_t relation_gen() {
  relation_t r;
  r.n = 0;
  return r;
}

// Utility function, always run after changing elements
void relation_sort(relation_t *r) {
  qsort(r->pairs, r->n, sizeof(r->pairs[0]), pair_cmp);
}

// Find exact pair, NULL if doesnt exist
pair *relation_find_exact(const relation_t *r, pair to_find) {
  void *p = bsearch(&to_find, r->pairs, r->n, sizeof(r->pairs[0]), pair_cmp);
  return (pair *)p;
}

// Adds new pair to relation, returns new size
unsigned int relation_add(relation_t *r, pair p) {
  if (relation_find_exact(r, p) == NULL) { // Dont add duplicate pairs
    if (r->n + 1 >= ARR_LEN(r->pairs))
      abort();
    r->pairs[r->n++] = p;
    relation_sort(r);
  }
  return r->n;
}

// Reads size N of relation, then N pairs
unsigned int relation_read(relation_t *r) {
  scanf("%ud", &r->n);
  for (unsigned int i = 0; i < r->n; i++) {
    pair *p = &r->pairs[i];
    scanf("%d %d", &p->first, &p->second);
  }
  relation_sort(r);
  return r->n;
}

// Returns the range in which first elements of relation are equal
// In the form [a,b-1]
pair relation_find_matching_first(const relation_t *r, int first) {
  pair res = pair_gen(0, 0);
  pair to_find = pair_gen(first, 0);
  pair *found = (pair *)bsearch(&to_find, r->pairs, r->n, sizeof(r->pairs[0]),
                                pair_cmp_first_only);
  if (found == NULL)
    return res;
  res.first = found - r->pairs;
  res.second = res.first;
  int *ix = &res.first;
  // Find lowest index
  while (*ix > 0 && r->pairs[*ix - 1].first == first)
    (*ix)--;
  // Find one above highest index
  ix = &res.second;
  while (*ix < (int)r->n && r->pairs[*ix].first == first)
    (*ix)++;
  return res;
}

// Relation must be sorted by second for this to work
pair relation_find_matching_second(const relation_t *r, int second) {
  pair res = pair_gen(0, 0);
  pair to_find = pair_gen(0, second);
  pair *found = (pair *)bsearch(&to_find, r->pairs, r->n, sizeof(r->pairs[0]),
                                pair_cmp_second_only);
  if (found == NULL)
    return res;
  res.first = found - r->pairs;
  res.second = res.first;
  int *ix = &res.second;
  // Find lowest index
  while (*ix > 0 && r->pairs[*ix - 1].second == second)
    (*ix)--;
  // Find one above highest index
  ix = &res.second;
  while (*ix < (int)r->n && r->pairs[*ix].second == second)
    (*ix)++;
  return res;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
cbool is_reflexive(const relation_t *r);
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
cbool is_irreflexive(const relation_t *r);
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
cbool is_symmetric(const relation_t *r);
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
cbool is_antisymmetric(const relation_t *r);
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
cbool is_asymmetric(const relation_t *r);
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
cbool is_transitive(const relation_t *r);

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
cbool is_partial_order(const relation_t *r);
// A total order relation is a partial order relation that is connected
cbool is_total_order(const relation_t *r);
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
cbool is_connected(const relation_t *r);
int find_max_elements(const relation_t *r, int[MAX_RANGE]);
int find_min_elements(relation_t *r, int[MAX_RANGE]);
int get_domain(const relation_t *r, set_t *s);

// Case 3:
// Returns size of relation RESULT, created from (R o S)
int composition(const relation_t *R, const relation_t *S, relation_t *result);

void print_int_array(int *array, int n) {
  printf("%d\n", n);
  for (int i = 0; i < n; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

int main(void) {
  int to_do;
  relation_t relation = relation_gen();
  relation_t relation_2 = relation_gen();
  relation_t comp_relation = relation_gen();
  set_t domain = set_gen();

  scanf("%d", &to_do);
  relation_read(&relation);
  int ordered, n_domain;

  switch (to_do) {
  case 1: {
    printf("%d ", is_reflexive(&relation));
    printf("%d ", is_irreflexive(&relation));
    printf("%d ", is_symmetric(&relation));
    printf("%d ", is_antisymmetric(&relation));
    printf("%d ", is_asymmetric(&relation));
    printf("%d\n", is_transitive(&relation));
    break;
  }
  case 2: {
    ordered = is_partial_order(&relation);
    n_domain = get_domain(&relation, &domain);
    printf("%d %d\n", ordered, is_total_order(&relation));
    print_int_array(domain.v, n_domain);
    if (!ordered)
      break;
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];
    int no_max_elements = find_max_elements(&relation, max_elements);
    int no_min_elements = find_min_elements(&relation, min_elements);
    print_int_array(max_elements, no_max_elements);
    print_int_array(min_elements, no_min_elements);
    break;
  }
  case 3: {
    relation_read(&relation_2);
    printf("%d\n",
           composition(&relation_2, &relation,
                       &comp_relation)); // Order of arguments must be swapped
    break;
  }
  default: {
    printf("NOTHING TO DO FOR %d\n", to_do);
    break;
  }
  }
  return 0;
}

// The relation R is reflexive if xRx for every x in X
cbool is_reflexive(const relation_t *r) {
  // if relation if reflexive, then there is a pair xRx for every x in any pair
  for (unsigned int i = 0; i < r->n; i++) {
    int a = r->pairs[i].first;
    int b = r->pairs[i].second;
    if (a == b)
      continue;
    int vals[2] = {a, b};
    for (int pair_ix = 0; pair_ix < 2; pair_ix++) {
      int v = vals[pair_ix];
      pair range = relation_find_matching_first(r, v);
      cbool found = false;
      for (int j = range.first; j < range.second; j++) {
        pair p = r->pairs[j];
        if (p.first == v && p.second == v) {
          found = true;
          break;
        }
      }
      if (!found)
        return false;
    }
  }
  return true;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
cbool is_irreflexive(const relation_t *r) {
  for (unsigned int i = 0; i < r->n; i++) {
    int a = r->pairs[i].first;
    int b = r->pairs[i].second;
    if (a == b)
      return false;
    int vals[2] = {a, b};
    for (int pair_ix = 0; pair_ix < 2; pair_ix++) {
      int v = vals[pair_ix];
      pair range = relation_find_matching_first(r, v);
      for (int j = range.first; j < range.second; j++) {
        pair p = r->pairs[j];
        if (p.first == v && p.second == v)
          return false;
      }
    }
  }
  return true;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
cbool is_symmetric(const relation_t *r) {
  for (unsigned int i = 0; i < r->n; i++) {
    // all pairs have a reflection
    if (relation_find_exact(r, pair_reverse(r->pairs[i])) == NULL)
      return false;
  }
  return true;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
cbool is_antisymmetric(const relation_t *r) {
  for (unsigned int i = 0; i < r->n; i++) {
    pair xRy = r->pairs[i];
    pair *yRx = relation_find_exact(r, pair_reverse(xRy));
    if (yRx == NULL)
      continue;
    if (!pair_equal(pair_reverse(xRy), *yRx))
      return false;
  }
  return true;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
cbool is_asymmetric(const relation_t *r) {
  for (unsigned int i = 0; i < r->n; i++) {
    pair xRy = r->pairs[i];
    pair *yRx = relation_find_exact(r, pair_reverse(xRy));
    if (yRx != NULL) // Cannot exist
      return false;
  }
  return true;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
cbool is_transitive(const relation_t *r) {
  for (unsigned int i = 0; i < r->n; i++) {
    pair xRy = r->pairs[i];
    int x = xRy.first;
    int y = xRy.second;
    pair range = relation_find_matching_first(r, y);
    for (int j = range.first; j < range.second; j++) {
      pair yRz = r->pairs[j];
      int z = yRz.second;
      pair xRz = pair_gen(x, z);
      if (relation_find_exact(r, xRz) == NULL)
        return false;
    }
  }
  return true;
}

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
cbool is_partial_order(const relation_t *r) {
  return is_reflexive(r) && is_transitive(r) && is_antisymmetric(r);
}

// A total order relation is a partial order relation that is connected
cbool is_total_order(const relation_t *r) {
  return is_partial_order(r) && is_connected(r);
}

int get_domain(const relation_t *r, set_t *s) {
  for (int i = 0; i < (int)r->n; i++) {
    pair p = r->pairs[i];
    int vals[2] = {p.first, p.second};
    for (int pair_ix = 0; pair_ix < 2; pair_ix++) {
      int v = vals[pair_ix];
      set_add(s, v);
    }
  }
  return s->n;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
cbool is_connected(const relation_t *r) {
  set_t domain = set_gen();
  int domain_size = get_domain(r, &domain);
  for (int i = 0; i < domain_size; i++) {
    for (int j = i; j < domain_size; j++) {
      int x = domain.v[i];
      int y = domain.v[j];
      if ((relation_find_exact(r, pair_gen(x, y)) == NULL) &&
          (relation_find_exact(r, pair_gen(y, x)) == NULL))
        return false;
    }
  }
  return true;
}

// Greatest elements x are not in relations of form xRy,
// except with itself (xRx)
int find_max_elements(const relation_t *r, int arr[MAX_RANGE]) {
  int arr_size = 0;
  set_t domain = set_gen();
  int domain_size = get_domain(r, &domain);
  for (int i = 0; i < domain_size; i++) {
    int x = domain.v[i];
    pair range = relation_find_matching_first(r, x);
    cbool all_xRx = true;
    for (int j = range.first; j < range.second; j++) {
      pair e = r->pairs[j];
      if (e.first != e.second) {
        all_xRx = false;
        break;
      }
    }
    if (all_xRx == false) // There exist relations xRy
      continue;
    arr[arr_size++] = x;
  }
  return arr_size;
}

// Smallest elements x are not in relation of form yRx,
// But can be in relation with themselves (xRx)
int find_min_elements(relation_t *r, int arr[MAX_RANGE]) {
  int arr_size = 0;
  set_t domain = set_gen();
  int domain_size = get_domain(r, &domain);
  qsort(r->pairs, r->n, sizeof(r->pairs[0]), int_cmp);
  for (int i = 0; i < domain_size; i++) {
    int x = domain.v[i];
    pair range = relation_find_matching_second(r, x);
    cbool all_xRx = true;
    for (int j = range.first; j < range.second; j++) {
      pair e = r->pairs[j];
      if (e.first != e.second) {
        all_xRx = false;
        break;
      }
    }
    if (all_xRx == false) // There exist relations yRx
      continue;
    arr[arr_size++] = x;
  }
  relation_sort(r);
  return arr_size;
}

// Returns size of relation RESULT, created from (R o S)
int composition(const relation_t *R, const relation_t *S, relation_t *result) {
  for (int i = 0; i < (int)S->n; i++) {
    int x = S->pairs[i].first;
    int y = S->pairs[i].second;
    pair range = relation_find_matching_first(R, y);
    for (int j = range.first; j < range.second; j++) {
      int z = R->pairs[j].second;
      relation_add(result, pair_gen(x, z));
    }
  }
  return result->n;
}
