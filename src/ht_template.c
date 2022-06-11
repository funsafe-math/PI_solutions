#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR -1
#define LIST_ERROR -2
#define PROGRAM_ERROR -3

typedef union data_union {
  int int_data;
  char char_data;
  void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union *);
typedef int (*CompareDataFp)(data_union, data_union);
typedef int (*HashFp)(data_union, int);

typedef struct ht_element {
  struct ht_element *next;
  data_union data;
} ht_element;

ht_element *new_ht_element(data_union data, ht_element *next) {
  ht_element *p = (ht_element *)malloc(sizeof(ht_element));
  p->data = data;
  p->next = next;
  return p;
}

typedef struct hash_table {
  int size;
  int no_elements;
  ht_element **ht;
  DataFp dump_data;
  DataFp free_data;
  CompareDataFp compare_data;
  HashFp hash_function;
  DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr)
    return ptr;
  exit(MEMORY_ALLOCATION_ERROR);
}

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
             CompareDataFp compare_data, HashFp hash_function,
             DataPFp modify_data) {
  p_table->size = size;
  p_table->no_elements = 0;
  p_table->ht = (ht_element **)calloc(size, sizeof(ht_element *));
  p_table->dump_data = dump_data;
  p_table->free_data = free_data;
  p_table->compare_data = compare_data;
  p_table->hash_function = hash_function;
  p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table *p_table, int n) {
  const ht_element *e = p_table->ht[n];
  while (e) {
    p_table->dump_data(e->data);
    e = e->next;
  }
  putchar('\n');
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
  free_data(to_delete->data);
}

// free all elements from the table (and the table itself)
void free_table(hash_table *p_table) {
  // Free nodes
  for (int i = 0; i < p_table->size; i++) {
    ht_element *e = p_table->ht[i];
    while (e) {
      if (p_table->free_data != NULL)
        p_table->free_data(e->data);
      ht_element *to_be_removed = e;
      e = e->next;
      free(to_be_removed);
    }
  }

  // Free the table
  free(p_table->ht);
}

// calculate hash function for integer k
int hash_base(int k, int size) {
  static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
  double tmp = k * c;
  return (int)floor(size * (tmp - floor(tmp)));
}

void insert_node(hash_table *p_table, ht_element *n) {
  p_table->no_elements++;
  n->next = NULL;
  int hash = p_table->hash_function(n->data, p_table->size);
  ht_element *e = p_table->ht[hash];
  n->next = e;
  p_table->ht[hash] = n;
  //   if (e == NULL) {
  //     p_table->ht[hash] = n;
  //     return;
  //   }
  //   while (e->next != NULL) {
  //     e = e->next;
  //   }
  //   e->next = n;
}

void rehash(hash_table *p_table) {
  int old_size = p_table->size;
  ht_element **old_arr = p_table->ht;
  p_table->size *= 2;
  p_table->no_elements = 0;
  p_table->ht = (ht_element **)calloc(p_table->size, sizeof(ht_element *));
  for (int i = 0; i < old_size; i++) {
    ht_element *n = old_arr[i];
    while (n) {
      ht_element *next = n->next;
      insert_node(p_table, n);
      n = next;
    }
  }
  free(old_arr);
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
  int hash = p_table->hash_function(*data, p_table->size);
  ht_element *e = p_table->ht[hash];
  while (e) {
    if (p_table->compare_data(e->data, *data) == 0) {
      return e;
    }
    e = e->next;
  }
  return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
  if (get_element(p_table, data)) {
    if (p_table->free_data)
      p_table->free_data(*data);
    return;
  }
  insert_node(p_table, new_ht_element(*data, NULL));
  if ((double)p_table->no_elements / p_table->size > MAX_RATE)
    rehash(p_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
  p_table->no_elements--;
  int hash = p_table->hash_function(data, p_table->size);
  ht_element fake_root;
  fake_root.next = p_table->ht[hash];
  ht_element *e = &fake_root;
  while (e->next) {
    ht_element *next = e->next;
    if (p_table->compare_data(next->data, data) == 0) {
      e->next = next->next;
      if (p_table->free_data != NULL)
        p_table->free_data(next->data);
      free(next);
      p_table->ht[hash] = fake_root.next;
      return;
    }
    e = next;
  }
  //   abort(); // Key does not exist
}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
  return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
  printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
  return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int() {
  int i;
  scanf("%d", &i);
  data_union u;
  u.int_data = i;
  return u;
}

// char element

int hash_char(data_union data, int size) {
  return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
  printf(" %c", data.char_data);
}

int cmp_char(data_union a, data_union b) {
  return a.char_data - b.char_data;
}

// read char value and insert to the union
data_union create_data_char() {
  char c;
  scanf(" %c", &c);
  data_union u;
  u.char_data = c;
  return u;
}

// Word element

typedef struct DataWord {
  char *word;
  int counter;
} DataWord;

void dump_word(data_union data) {
  DataWord *p = (DataWord *)data.ptr_data;
  printf("%s %d ", p->word, p->counter);
}

void free_word(data_union data) {
  DataWord *p = (DataWord *)data.ptr_data;
  free(p->word);
  free(p);
}

int cmp_word(data_union a, data_union b) {
  DataWord *first = (DataWord *)a.ptr_data;
  DataWord *second = (DataWord *)b.ptr_data;
  return strcmp(first->word, second->word);
}

int hash_word(data_union data, int size) {
  int s = 0;
  DataWord *dw = (DataWord *)data.ptr_data;
  char *p = dw->word;
  while (*p) {
    s += *p++;
  }
  return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
  DataWord *p = (DataWord *)data->ptr_data;
  p->counter++;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *str) {
  DataWord *p = (DataWord *)malloc(sizeof(DataWord));
  p->word = strdup(str);
  p->counter = 1;

  data_union u;
  u.ptr_data = p;
  return u;
}

void to_lowercase(char *s) {
  for (; *s; ++s)
    *s = tolower(*s);
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
  const char delimiters[] = ",.?!:;-\t\n\b\r ";
  char *line = NULL;
  ssize_t line_len = 0;
  size_t len;
  while ((line_len = getline(&line, &len, stream)) > 0) {
    char *token = NULL;
    for (token = strtok(line, delimiters); token != NULL;
         token = strtok(NULL, delimiters)) {
      to_lowercase(token);
      data_union data = create_data_word(token);
      ht_element *e;
      if ((e = get_element(p_table, &data)) != NULL) {
        modify_word(&e->data);
      } else {
        insert_element(p_table, &data);
      }
    }
    free(line);
    line = NULL;
  }
  free(line);
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
  char op;
  data_union data;
  for (int i = 0; i < n; ++i) {
    scanf(" %c", &op);
    data = create_data();
    switch (op) {
    case 'r':
      remove_element(p_table, data);
      break;
    case 'i':
      insert_element(p_table, &data);
      break;
    default:
      printf("No such operation: %c\n", op);
      break;
    }
  }
}

int main(void) {
  int to_do, n, index;
  hash_table table;
  char buffer[BUFFER_SIZE];
  data_union data;

  scanf("%d", &to_do);
  switch (to_do) {
  case 1: // test integer table
  {
    scanf("%d %d", &n, &index);
    init_ht(&table, 4, dump_int, NULL, cmp_int, hash_int, NULL);
    table_test(&table, n, create_data_int);
    printf("%d\n", table.size);
    dump_list(&table, index);
    free_table(&table);
    break;
  }
  case 2: // test char table
  {
    scanf("%d %d", &n, &index);
    init_ht(&table, 4, dump_char, NULL, cmp_char, hash_char, NULL);
    table_test(&table, n, create_data_char);
    printf("%d\n", table.size);
    dump_list(&table, index);
    free_table(&table);
    break;
  }
  case 3: // read words from text, insert into table, and print
  {
    scanf("%s", buffer);
    init_ht(&table, 8, dump_word, free_word, cmp_word, hash_word, modify_word);
    stream_to_ht(&table, stdin);
    printf("%d\n", table.size);
    data = create_data_word(buffer);
    ht_element *e = get_element(&table, &data);
    if (e)
      table.dump_data(e->data);
    if (table.free_data)
      table.free_data(data);
    free_table(&table);
    break;
  }
  default: {
    printf("NOTHING TO DO FOR %d\n", to_do);
    break;
  }
  }
  return 0;
}
