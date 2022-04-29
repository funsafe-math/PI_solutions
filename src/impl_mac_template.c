#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAB_SIZE 1000
#define BUF_SIZE 1000
#ifndef TEST
#define TEST 0
#endif
#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))

// Funkcje dla punktu 8.1.1 Macierze prostokatne

// Copy matrix B into A
// x - rows
// y - columns
void copy_mat(const int x, const int y, double *A, const double *B) {
  size_t items = (size_t)x * y;
  memcpy(A, B, items * sizeof(A[0]));
}

//  Oblicza S = A + B
void sum_mat(const int x, const int y, const double *A, const double *B,
             double *S) {
  int items = x * y;
  for (int i = 0; i < items; i++) {
    S[i] = A[i] + B[i];
  }
}

double get(const int x, const int y, const int columns, const double *T) {
  size_t ix = x * columns + y;
  return T[ix];
}

void set(const int x, const int y, const int columns, double *T,
         const double val) {
  size_t ix = x * columns + y;
  T[ix] = val;
}

// computes product A*B = R
void prod_mat(const int rowsA, const int columnsA, const int columnsB,
              const double *A, const double *B, double *R) {
  // A : rowsA * columnsA
  // B : columnsA * columnsB
  // R : rowsA * columnsB

  for (int x = 0; x < rowsA; x++) {
    for (int z = 0; z < columnsB; z++) {
      double sum = 0;
      for (int y = 0; y < columnsA; y++) {
        sum += get(x, y, columnsA, A) * get(y, z, columnsB, B);
      }
      set(x, z, columnsB, R, sum);
    }
  }
}

void prod_mat_v2(const int, const int, const int, ...);
// Nalezy uzupelnic liste parametrow - z parametrami typu tablicy 1D
// W ciele funkcji nalezy zdefiniować wskazniki do tablic 2D, do ktorych nalezy
// przepisac (z odpowiednim rzutowaniem) parametry A, B i AB. W algorytmie
// mnozenia macierzy bedzie mozna stosowac notacje indeksowa X[i][j]

void prod_mat_v3(const int, const int, const int, ...);
// Nalezy uzupelnic liste parametrow - z parametrami typu tablicy 2D
// W algorytmie mnozenia macierzy bedzie mozna stosowac notacje indeksowa
// X[i][j]

void read_mat(const int rows, const int cols, double *t) {
  for (int i = 0; i < rows * cols; ++i, ++t)
    scanf("%lf", t);
}

void print_mat(int rows, int cols, double *t) {
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c)
      printf("%.2f ", *t++);
    printf("\n");
  }
}

// Funkcje do punktow 8.1.2, 8.1.3 i 8.1.4

typedef struct {
  char *p[BUF_SIZE];
  size_t n;
} contiguos_arr_s_t;

void contiguos_arr_s_add(contiguos_arr_s_t *arr, char *val) {
  if (arr->n >= ARR_LEN(arr->p) - 1)
    abort();
  arr->p[arr->n++] = val;
}

contiguos_arr_s_t strings;
// 8.1.3.  Funkcje dla tablicy wierszy znakowych
// Wskazane uzycie funkcji fgets(), memchr(), malloc()
int read_char_lines(char *tab[], int lines_count, FILE *stream) {
  size_t len = 0;
  char *str = NULL;
  int str_l = 0;
  if (TEST) {
    getline(&str, &len, stream);
    free(str);
    str = NULL;
  }
  while ((str_l = getline(&str, &len, stream)) > 0) {
    str[str_l - 1] = 0; // remove trailing '\n'
    tab[lines_count++] = str;
    str = NULL;
  }
  return lines_count;
}

void write_char_line(char *tab[], int n) {
  puts(tab[n + 1]);
}

// Nalezy pamietac o zwolnieniu przydzialonej pamieci
void delete_lines(char *tab[], int line_count) {
  for (int i = 0; i < line_count; i++) {
    free(tab[i]);
  }
}

typedef struct {
  double p[BUF_SIZE];
  size_t n;
} contiguos_arr_d_t;

void contiguos_arr_d_add(contiguos_arr_d_t *arr, double val) {
  if (arr->n >= ARR_LEN(arr->p) - 1)
    abort();
  arr->p[arr->n++] = val;
}

contiguos_arr_d_t doubles;

// 8.1.2.  Funkcje dla tablicy wierszy liczbowych w obszarze ciągłym
// Wskazane uzycie funkcji fgets(), strtod().
int read_dbl_lines_v1(double *ptr_tab[], int lines_counter, FILE *stream) {
  char buffer[BUF_SIZE] = "";
  int ix = 0;
  int c;
  ptr_tab[lines_counter++] = &doubles.p[0];
  while ((c = fgetc(stream)) != EOF) {
    if (isspace(c)) {
      if (ix == 0)
        continue;
      buffer[ix] = 0;
      char *end;
      double v = strtod(buffer, &end);
      contiguos_arr_d_add(&doubles, v);
      ix = 0;
      if (c == '\r') {
        c = fgetc(stream);
      }
      if (c == '\n') {
        // if (doubles.n == 0)
        //   continue;
        ptr_tab[lines_counter++] = &doubles.p[doubles.n];
      }
    } else {
      buffer[ix++] = c;
    }
  }
  return lines_counter - 1;
}

void write_dbl_line_v1(double *ptr_tab[], int n) {
  n -= 1; // To start counting from 1
  if (TEST)
    n--;
  double *p = ptr_tab[n];
  while (p != ptr_tab[n + 1]) {
    printf("%lf ", *p);
    p++;
    if (p >= doubles.p + doubles.n) {
      abort();
    }
  }
}

// 8.1.4.  Funkcje dla tablicy wierszy liczbowych w odrebnych obszarach
// Wskazane uzycie funkcji fgets(), strtod(), malloc().

struct line {
  double *values;
  int len;
  double average;
};

typedef struct line line_type;

int read_dbl_lines(line_type lines_tab[], int line_count, FILE *stream) {
  double buf[TAB_SIZE];
  int ix = 0;
  char *line = NULL;
  int line_len = 0;
  size_t len;
  while ((line_len = getline(&line, &len, stream)) > 0) {
    if (line_len == 1) {
      free(line);
      line = NULL;
      continue;
    }
    char *begin = line;
    char *end = begin;
    double sum = 0;
    while (end - line <= line_len) {
      double v = strtod(begin, &end);
      if (v == 0)
        break;
      begin = end;
      buf[ix++] = v;
      sum += v;
    }
    line_type *lt = &lines_tab[line_count++];
    lt->len = ix;
    const long byte_len = lt->len * sizeof(lt->values[0]);
    lt->values = (double *)malloc(byte_len);
    memcpy(lt->values, buf, byte_len);
    lt->average = sum / lt->len;

    ix = 0;
    free(line);
    line = NULL;
  }
  free(line);
  return line_count;
}

void write_dbl_line(line_type lines_tab[], int n) {
  line_type line = lines_tab[n];
  for (int i = 0; i < line.len; i++) {
    printf("%lf ", line.values[i]);
  }
  printf("\n%lf ", line.average);
}

void delete_dbl_lines(line_type lines_tab[], int n) {
  for (int i = 0; i < n; i++) {
    free(lines_tab[i].values);
  }
}

int cmp(const void *a, const void *b) {
  line_type *first = (line_type *)a;
  line_type *second = (line_type *)b;
  return first->average - second->average;
}

// Nalezy zastosowac funkcje qsort() z wywolaniem funkcji cmp().
void sort_by_average(line_type lines_tab[], int line_count) {
  qsort(lines_tab, line_count, sizeof(lines_tab[0]), cmp);
}

/////////////////////////////////////////////////////////////////////////

int main(void) {
  FILE *stream;
  char file_name[50];
  int to_do;
  if (TEST)
    printf("Wpisz nr zadania ");
  scanf("%d", &to_do);
  stream = stdin;

  double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
  int n, lines_counter, rowsA, colsA, rowsB, colsB;
  char *char_lines_table[TAB_SIZE];
  double series_table[TAB_SIZE];
  double *ptr_table[TAB_SIZE];
  line_type dbl_lines_table[TAB_SIZE];

  switch (to_do) {
  case 1: // Macierze prostkatne
  {
    if (TEST)
      printf("Wpisz liczbe wierszy i kolumn oraz elementy 1. macierzy ");
    scanf("%d %d", &rowsA, &colsA);
    read_mat(rowsA, colsA, A);
    if (TEST)
      printf("Wpisz liczbe wierszy i kolumn oraz elementy 2. macierzy ");
    scanf("%d %d", &rowsB, &colsB);
    read_mat(rowsB, colsB, B);
    prod_mat(rowsA, colsA, colsB, A, B, C);
    print_mat(rowsA, colsB, C);
    break;
  }
  case 2: // Ciagi liczbowe w ciaglym obszarze, wiersze roznej dlugosci:
  {       // ptr_table[lines_counter] points out 1. byte of free memory
    ptr_table[0] = series_table;
    if (TEST)
      printf("Wpisz numer wiersza (liczony od 1) ");
    scanf("%d", &n);
    if (TEST)
      printf("Wpisz kolejne linie zawierajace liczby, zakoncz znakiem EOF\n");
    if (TEST == 0) {
      scanf("%50s", file_name);
      stream = fopen(file_name, "r");
      if (stream == NULL) {
        printf("fopen failed\n");
        return -1;
      }
    }
    lines_counter = read_dbl_lines_v1(ptr_table, 0, stream);
    write_dbl_line_v1(ptr_table, n);
    break;
  }
  case 3: // Linie znakow w odrebnych obszarach pamieci:
  {
    if (TEST)
      printf("Wpisz numer wiersza (liczony od 1) ");
    scanf("%d", &n);
    if (TEST)
      printf("Wpisz kolejne linie zawierajace znaki ASCII, zakoncz znakiem "
             "EOF\n");
    if (TEST == 0) {
      scanf("%50s", file_name);
      stream = fopen(file_name, "r");
      if (stream == NULL) {
        printf("fopen failed\n");
        return -1;
      }
    }
    lines_counter = read_char_lines(char_lines_table, 0, stream);
    write_char_line(char_lines_table, n - 2);
    delete_lines(char_lines_table, lines_counter);
    break;
  }
  case 4: // Ciagi liczbowe w odrebnych obszarach pamieci + sortowanie:
  {
    if (TEST)
      printf("Wpisz numer wiersza (liczony od 1) w kolejnosci rosnacej "
             "sredniej ");
    scanf("%d", &n);
    if (TEST)
      printf("Wpisz kolejne linie zawierajace liczby, zakoncz znakiem EOF\n");
    if (TEST == 0) {
      scanf("%s", file_name);
      stream = fopen(file_name, "r");
      if (stream == NULL) {
        printf("fopen failed\n");
        return -1;
      }
    }
    lines_counter = read_dbl_lines(dbl_lines_table, 0, stream);
    sort_by_average(dbl_lines_table, lines_counter);
    write_dbl_line(dbl_lines_table, n - 1);
    delete_dbl_lines(dbl_lines_table, lines_counter);
    break;
  }
  default: {
    printf("NOTHING TO DO FOR %d\n", to_do);
  }
  }
  return 0;
}
