#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define ARR_LEN(array) (sizeof(array) / sizeof(array[0]))
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

#ifndef TEST
#define TEST 0 // 1 dla testowania, 0 dla automatycznej oceny
#endif

int count[MAX_DIGRAMS] = {0};

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp(const void *a, const void *b) {
  int va = *(int *)a;
  int vb = *(int *)b;
  if (count[va] == count[vb])
    return va - vb; // sort alphabetically if counts equal
  return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di(const void *a, const void *b) {
  int va = *(int *)a;
  int vb = *(int *)b;
  // sort according to second char if counts and the first char equal
  if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS)
    return va % MAX_CHARS - vb % MAX_CHARS;
  // sort according to first char if counts equal
  if (count[va] == count[vb])
    return va / MAX_CHARS - vb / MAX_CHARS;
  return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stream
void wc(int *nl, int *nw, int *nc, FILE *stream) {
  *nc = 0;
  *nw = 0;
  *nl = 0;
  char in_word = 0;

  for (int c = fgetc(stream); c != EOF; c = fgetc(stream)) {
    (*nc)++;
    if (c == '\n')
      (*nl)++;
    if (isspace(c)) {
      in_word = 0;
    } else if (!in_word) {
      (*nw)++;
      in_word = 1;
    }
  }
}

void insert_sort(int arr[], int n, char (*pred)(int, int, void *), void *data) {
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      // Predecate should be true everywhere in sorted array
      if (!pred(arr[i], arr[j], data)) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
      }
    }
  }
}

char char_count_predecate(int a, int b, void *data) {
  int *counts = (int *)data;
  if (counts[a] < counts[b] || (counts[a] == counts[b] && b > a))
    return 0; // In this case should swap
  return 1;
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stream. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt, FILE *stream) {
  int counts[LAST_CHAR - FIRST_CHAR] = {
      0,
  };
  memset(counts, 0, sizeof(counts));
  int c;
  while ((c = fgetc(stream)) != EOF) {
    if (c >= LAST_CHAR || c < FIRST_CHAR)
      continue;
    counts[c - FIRST_CHAR]++;
  }

  // Sort an ix array in decreasing order
  int ixes[LAST_CHAR - FIRST_CHAR];
  for (unsigned int i = 0; i < ARR_LEN(ixes); i++) {
    ixes[i] = i;
  }
  insert_sort(ixes, ARR_LEN(ixes), char_count_predecate, counts);
  // qsort_r(ixes, ARR_LEN(ixes), sizeof(ixes[0]), cmp, counts);

  // Set results
  *n_char = ixes[char_no - 1] + FIRST_CHAR;
  *cnt = counts[ixes[char_no - 1]];
}

char digram_count_predecate(int a, int b, void *data) {
  int *counts = (int *)data;
  if (counts[a] < counts[b] || (counts[a] == counts[b] && (b > a)))
    return 0; // In this case should swap
  return 1;
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stream. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[], FILE *stream) {
  int previous = fgetc(stream);
  int c;
  while ((c = fgetc(stream)) != EOF) {
    if (c < FIRST_CHAR || c >= LAST_CHAR || previous < FIRST_CHAR ||
        previous >= LAST_CHAR) {
      previous = c;
      continue;
    }
    count[(previous - FIRST_CHAR) * MAX_CHARS + c - FIRST_CHAR]++;
    previous = c;
  }

  // 4*8kB is a lot to put on a stack
  int *ixes = (int *)malloc(MAX_DIGRAMS * sizeof(int));
  for (int i = 0; i < MAX_DIGRAMS; i++) {
    ixes[i] = i;
  }

  // insert_sort(ixes, MAX_DIGRAMS, digram_count_predecate, digram_count);
  // qsort_r(ixes, ARR_LEN(ixes), sizeof(ixes[0]), cmp_di, digram_count);
  qsort(ixes, MAX_DIGRAMS, sizeof(int), cmp_di);

  // Set results
  digram[0] = (ixes[digram_no - 1] / MAX_CHARS) + FIRST_CHAR;
  digram[1] = (ixes[digram_no - 1] % MAX_CHARS) + FIRST_CHAR;
  digram[2] = count[ixes[digram_no - 1]];

  // The Lord giveth, and the Lord taketh away
  free(ixes);
}

int equal_digrams(const int a[2], const char b[2]) {
  return a[0] == b[0] && a[1] == b[1];
}

// Count block and line comments in the text read from stream. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter,
                   FILE *stream) {
  *line_comment_counter = 0;
  *block_comment_counter = 0;
  int comment_style = 0;
  int chars[2];
  chars[0] = fgetc(stream);
  while ((chars[1] = fgetc(stream)) != EOF) {
    switch (comment_style) {
    case IN_BLOCK_COMMENT: {
      if (equal_digrams(chars, "*/")) {
        comment_style = 0;
        chars[1] = fgetc(stream);
      }
      break;
    }
    case IN_LINE_COMMENT: {
      if (chars[1] == '\n') {
        comment_style = 0;
      }
      break;
    }
    default: {
      if (equal_digrams(chars, "//")) {
        comment_style = IN_LINE_COMMENT;
        (*line_comment_counter)++;
        chars[1] = fgetc(stream);
      } else if (equal_digrams(chars, "/*")) {
        comment_style = IN_BLOCK_COMMENT;
        (*block_comment_counter)++;
        chars[1] = fgetc(stream);
      }
    }
    }
    chars[0] = chars[1];
  }
}

int main(void) {
  int to_do;
  int nl, nw, nc, char_no, n_char, cnt;
  int line_comment_counter, block_comment_counter;
  int digram[3];

  char file_name[40];
  FILE *stream;

  if (TEST)
    printf("Wpisz nr zadania ");
  scanf("%d", &to_do);
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

  //	to_do = read_line();
  switch (to_do) {
  case 1: // wc()
    wc(&nl, &nw, &nc, stream);
    printf("%d %d %d\n", nl, nw, nc);
    break;
  case 2: // char_count()
          //			char_no = read_line();
    if (TEST)
      printf("Wpisz numer znaku ");
    scanf("%d", &char_no);
    char_count(char_no, &n_char, &cnt, stream);
    printf("%c %d\n", n_char, cnt);
    break;
  case 3: // digram_count()
          //			char_no = read_line();
    if (TEST)
      printf("Wpisz numer digramu ");
    scanf("%d", &char_no);
    digram_count(char_no, digram, stream);
    printf("%c%c %d\n", digram[0], digram[1], digram[2]);
    break;
  case 4:
    find_comments(&line_comment_counter, &block_comment_counter, stream);
    printf("%d %d\n", block_comment_counter, line_comment_counter);
    break;
  default:
    printf("NOTHING TO DO FOR %d\n", to_do);
    break;
  }

  return 0;
}
