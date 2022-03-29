#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#ifndef TEST
#define TEST 0 // 1 - dla testowania,  0 - dla automatycznej oceny
#endif

#define true 1
#define false 0
typedef unsigned char cbool;

#define ARR_LEN(ARR) sizeof(ARR) / sizeof(ARR[0])

int index_cmp(const void *, const void *);
int cmp(const void *, const void *);
int cmp_search(const void *first, const void *second);
int cmp_strings(const void *first, const void *second);

char tab[MAX_IDS][MAX_ID_LEN];
int tab_size = 0;

char *keywords[] = {"auto",     "break",   "case",   "char",     "const",
                    "continue", "default", "do",     "double",   "else",
                    "enum",     "extern",  "float",  "for",      "goto",
                    "if",       "int",     "long",   "register", "return",
                    "short",    "signed",  "sizeof", "static",   "struct",
                    "switch",   "typedef", "union",  "unsigned", "void",
                    "volatile", "while"};

struct string {
  int len;
  char buf[100];
};
typedef struct string string_t;

string_t string_create(void) {
  string_t ret;
  ret.buf[0] = 0;
  ret.len = 0;
  return ret;
}
void string_append(string_t *str, const char c) {
  if (str->len == sizeof(str->buf) - 2) // reserve place for null-termination
    abort();
  str->buf[str->len] = c;
  str->len++;
  str->buf[str->len] = 0;
}
void string_clear(string_t *str) {
  str->buf[0] = 0;
  str->len = 0;
}
void string_pop(string_t *str) {
  if (str->len == 0)
    return;
  str->len--;
  str->buf[str->len] = 0;
}

// return pointer to last N characters of STR
const char *string_last_n_chars(const string_t *str, int n) {
  int offset = str->len - n;
  if (offset < 0)
    offset = 0;
  return str->buf + offset;
}

cbool string_endswith(const string_t *str, const char *end) {
  int ending_len = strlen(end);
  const char *substr = string_last_n_chars(str, ending_len);
  if (str->len < ending_len || strcmp(substr, end) != 0)
    return false;
  return true;
}
// Filter out backslashes
int get_char(FILE *stream) {
  int c = fgetc(stream);
  while (c == '\\') {
    fgetc(stream);
    c = fgetc(stream);
  }
  return c;
}

cbool is_id_seperator(int c) {
  return (c != '_') && (isspace(c) || ispunct(c));
}

cbool is_id_start(int c) { return (c == '_') || isalpha(c); }
cbool is_id_mid(int c) { return (c == '_') || isalnum(c); }

cbool register_id(string_t *str, int *unique_id_counter) {
  if (!is_id_start(str->buf[0]))
    return false;
  for (int i = 1; i < str->len - 1; i++) {
    if (!is_id_mid(str->buf[i]))
      return false;
  }
  string_pop(str);
  void *ptr = bsearch(str->buf, keywords, ARR_LEN(keywords),
                      sizeof(keywords[0]), cmp_search);
  if (!ptr) {
    // ID not in keywords
    if (!bsearch(str->buf, tab, tab_size, sizeof(tab[0]), cmp_strings)) {
      if (tab_size == ARR_LEN(tab))
        abort();
      strcpy(tab[tab_size], str->buf);
      ++tab_size;
      qsort(tab, tab_size, sizeof(tab[0]), cmp_strings);
      (*unique_id_counter)++;
    }
  }

  return true;
}

int find_idents(FILE *stream) {
  enum states {
    normal,
    block_comment,
    line_comment,
    string,
  } state = normal;
  int counter = 0;
  string_t token = string_create();
  string_t *str = &token;
  int c;
  while ((c = get_char(stream)) != EOF) {
    string_append(str, c);
    switch (state) {
    case block_comment: {
      if (string_endswith(str, "*/")) {
        string_clear(str);
        state = normal;
      }
      break;
    }
    case line_comment: {
      if (string_endswith(str, "\n")) {
        string_clear(str);
        state = normal;
      }
      break;
    }
    case string: {
      if (string_endswith(str, "\"")) {
        string_clear(str);
        state = normal;
      }
      break;
    }

    case normal:
      if (string_endswith(str, "/*")) {
        string_clear(str);
        state = block_comment;
      } else if (string_endswith(str, "//")) {
        string_clear(str);
        state = line_comment;
      } else if (string_endswith(str, "\"")) {
        string_clear(str);
        state = string;
      } else if (is_id_seperator(c) && !(str->len < 2 && c == '/')) {
        if (register_id(str, &counter)) {
          string_clear(str);
          string_append(str, c);
        }
      }
      if (isspace(c)) {
        string_clear(str);
      }
    }
  }
  return counter;
}

int cmp(const void *first_arg, const void *second_arg) {
  char *a = *(char **)first_arg;
  char *b = *(char **)second_arg;
  return strcmp(a, b);
}

int cmp_search(const void *first, const void *second) {
  char *a = (char *)first;
  char *b = *(char **)second;
  return strcmp(a, b);
}
int cmp_strings(const void *first, const void *second) {
  char *a = (char *)first;
  char *b = (char *)second;
  return strcmp(a, b);
}

int index_cmp(const void *first_arg, const void *second_arg) {
  int a = *(int *)first_arg;
  int b = *(int *)second_arg;
  return strcmp(tab[a], tab[b]);
}

int main(void) {
  char file_name[40];
  FILE *stream;

  if (TEST)
    stream = stdin;
  else {
    scanf("%s", file_name);
    stream = fopen(file_name, "r");
    if (stream == NULL) {
      printf("fopen failed\n");
      return -1;
    }
  }
  printf("%d\n", find_idents(stream));
  return 0;
}
