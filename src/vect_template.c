#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64

typedef struct Vector {
  void *data;
  size_t element_size;
  size_t size;
  size_t capacity;
} Vector;

typedef struct Person {
  int age;
  char first_name[MAX_STR_LEN];
  char last_name[MAX_STR_LEN];
} Person;

typedef struct {
  void *data;
  size_t len;
} Vector_elem;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
  vector->data = malloc(block_size * element_size);
  vector->element_size = element_size;
  vector->size = 0;
  vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
  if (new_capacity <= vector->capacity)
    return;
  size_t target_capacity = vector->capacity * 2;
  while (target_capacity < new_capacity)
    target_capacity *= 2;
  vector->data = realloc(vector->data, target_capacity * vector->element_size);
  vector->capacity = target_capacity;
}

size_t ptr_diff(void *start, void *end) {
  return (size_t)end - (size_t)start;
}

void *ptr_add(void *start, size_t offset) {
  char *s = (char *)start;
  return s + offset;
}

Vector_elem at_unsafe(Vector *vector, size_t ix) {
  Vector_elem e;
  e.data = ptr_add(vector->data, ix * vector->element_size);
  e.len = vector->element_size;
  return e;
}

Vector_elem at(Vector *vector, size_t ix) {
  if (ix >= vector->size)
    abort();
  return at_unsafe(vector, ix);
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
  reserve(vector, new_size);
  if (vector->size < new_size) {
    void *starting = at_unsafe(vector, vector->size).data;
    void *ending = at_unsafe(vector, new_size).data;
    memset(starting, 0, ptr_diff(starting, ending));
  }
  vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
  reserve(vector, vector->size + 1);
  void *last_element_start = at_unsafe(vector, vector->size).data;
  memcpy(last_element_start, value, vector->element_size);
  vector->size++;
}

// Remove all elements from the vector
void clear(Vector *vector) {
  vector->size = 0;
}

// Remove the last element from the vector
void pop_back(Vector *vector) {
  if (vector->size == 0)
    abort();
  vector->size--;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, int index, void *value) {
  reserve(vector, vector->size + 1);
  if ((size_t)index > vector->size)
    abort();
  void *new_elem = at_unsafe(vector, index).data;
  void *last_elem = at_unsafe(vector, vector->size).data;
  memmove(ptr_add(new_elem, +vector->element_size), new_elem,
          ptr_diff(new_elem, last_elem));
  memcpy(new_elem, value, vector->element_size);
  vector->size++;
}

// Erase element at position index
void erase(Vector *vector, int index) {
  if ((size_t)index >= vector->size)
    abort();
  void *del_elem = at(vector, index).data;
  memmove(del_elem, ptr_add(del_elem, vector->element_size),
          (vector->size - 1 - index) * vector->element_size);

  vector->size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value,
                 int (*cmp)(const void *, const void *)) {
  for (int i = vector->size - 1; i >= 0; i--) {
    if (cmp(at(vector, i).data, value) == 0) {
      erase(vector, i);
    }
  }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
  for (int i = vector->size - 1; i >= 0; i--) {
    if (predicate(at(vector, i).data)) {
      erase(vector, i);
    }
  }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
  size_t new_capacity = vector->size;
  if (new_capacity >= vector->capacity)
    return;
  vector->capacity = new_capacity;
  vector->data = realloc(vector->data, new_capacity * vector->element_size);
}

// Print integer vector
void print_vector_int(Vector *vector) {
  printf("%zu\n", vector->size);
  for (int i = 0; (size_t)i < vector->size; i++) {
    int *elem = (int *)at(vector, i).data;
    printf("%d ", *elem);
  }
  puts("\n");
}

// Print char vector
void print_vector_char(Vector *vector) {
  printf("%zu\n", vector->capacity);
  for (int i = 0; (size_t)i < vector->size; i++) {
    char *elem = (char *)at(vector, i).data;
    printf("%c ", *elem);
  }
  puts("\n");
}

void print_person(Person *person) {
  printf("%d %s %s\n", person->age, person->first_name, person->last_name);
}

// Print vector of Person
void print_vector_person(Vector *vector) {
  printf("%zu\n", vector->capacity);
  for (int i = 0; (size_t)i < vector->size; i++) {
    Person *elem = (Person *)at(vector, i).data;
    print_person(elem);
  }
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
  const int *first = (int *)v1;
  const int *second = (int *)v2;
  return *first - *second;
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
  const char *first = (char *)v1;
  const char *second = (char *)v2;
  return *first - *second;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
  const Person *first = (const Person *)p1;
  const Person *second = (const Person *)p2;
  if (first->age == second->age) {
    int first_res = strcmp(first->first_name, second->first_name);
    if (first_res == 0)
      return strcmp(first->last_name, second->last_name);
    return first_res;
  }
  return second->age - first->age;
}

// predicate: check if number is even
int is_even(void *value) {
  int *v = (int *)value;
  return !(*v & 1);
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
  char c = *(char *)value;
  char uppercase = c & 0x1f;
  return (0x2208222 >> (uppercase)) & 1;
}

// predicate: check if person is older than 25
int is_older_then_25(void *person) {
  const Person *p = (Person *)person;
  return p->age > 25;
}

// -------------------------------------------------------------

void read_int(void *value) {
  scanf("%d", (int *)value);
}

void read_char(void *value) {
  char c[2];
  scanf("%s", c);
  *(char *)value = c[0];
}

void read_person(void *value) {
  Person *person = (Person *)value;
  scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, int n, void (*read)(void *),
                 int (*cmp)(const void *, const void *),
                 int (*predicate)(void *)) {
  char op[2];
  int index;
  size_t size;
  void *v = malloc(vector->element_size);
  for (int i = 0; i < n; ++i) {
    scanf("%s", op);
    switch (op[0]) {
    case 'p': // push_back
      read(v);
      push_back(vector, v);
      break;
    case 'i': // insert
      scanf("%d", &index);
      read(v);
      insert(vector, index, v);
      break;
    case 'e': // erase
      scanf("%d", &index);
      read(v);
      erase(vector, index);
      erase_value(vector, v, cmp);
      break;
    case 'd': // erase (predicate)
      erase_if(vector, predicate);
      break;
    case 'r': // resize
      scanf("%zu", &size);
      resize(vector, size);
      break;
    case 'c': // clear
      clear(vector);
      break;
    case 'f': // shrink
      shrink_to_fit(vector);
      break;
    case 's': // sort
      qsort(vector->data, vector->size, vector->element_size, cmp);
      break;
    default:
      printf("No such operation: %s\n", op);
      break;
    }
  }
  free(v);
}

int main(void) {
  int to_do, n;
  Vector vector_int, vector_char, vector_person;

  scanf("%d%d", &to_do, &n);

  switch (to_do) {
  case 1:
    init_vector(&vector_int, 4, sizeof(int));
    vector_test(&vector_int, n, read_int, int_cmp, is_even);
    print_vector_int(&vector_int);
    free(vector_int.data);
    break;
  case 2:
    init_vector(&vector_char, 2, sizeof(char));
    vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
    print_vector_char(&vector_char);
    free(vector_char.data);
    break;
  case 3:
    init_vector(&vector_person, 2, sizeof(Person));
    vector_test(&vector_person, n, read_person, person_cmp, is_older_then_25);
    print_vector_person(&vector_person);
    free(vector_person.data);
    break;
  default:
    printf("Nothing to do for %d\n", to_do);
    break;
  }

  return 0;
}
