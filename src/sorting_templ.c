#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TEST
#define TEST 0
#endif

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

// Quick sort
void quick_sort(int *A, int len) {
  if (len < 2)
    return;
  int left = 0;
  int right = len - 1;
  int pivot = A[len / 2];
  while (left <= right) {
    while (A[left] < pivot) {
      left++;
    }
    while (A[right] > pivot) {
      right--;
    }
    if (left <= right) {
      swap(&A[left], &A[right]);
      left++;
      right--;
    }
  }
  quick_sort(A, right + 1);
  quick_sort(A + left, len - left);
}

int str_comparator(const void *a, const void *b) {
  const char *first = *(char **)a;
  const char *second = *(char **)b;
  return strcmp(first, second);
}

// Sortowanie słów
void sort_words(char *words[], int count) {
  qsort(words, count, sizeof(words[0]), str_comparator);
}

// Sortowanie leksykograficzne struktur

typedef struct structure {
  char *name;
  char *nazwa;
} structure;
structure s[10];

//...............
int structure_cmp(const void *a, const void *b) {
  structure *first = (structure *)a;
  structure *second = (structure *)b;
  return strcmp(first->name, second->name);
}

void struct_lexi_sort(int *len) {
  qsort(s, *len, sizeof(s[0]), structure_cmp);
}

// Złożone wymagania dotyczące sortowania

typedef struct struktura {
  char *sname;
  char *fname;
  int age;
} struktura;
struktura sa[11];

int is_female_name(const char *name) {
  int len = strlen(name);
  return name[len - 1] == 'a';
}

int struktura_cmp(const void *a, const void *b) {
  const struktura *first = (struktura *)a;
  const struktura *second = (struktura *)b;
  int first_female = is_female_name(first->sname);
  int second_female = is_female_name(second->sname);
  if (first_female && second_female) {
    return first->age - second->age;
  } else if (first_female || second_female) {
    return second_female - first_female;
  } else {
    return strcmp(first->fname, second->fname);
  }
}

void struct_name_sort(int *len) {
  qsort(sa, *len, sizeof(sa[0]), struktura_cmp);
}

///////////////////////////////////////////////////

int main(void) {
  int n = 10;
  int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
  char *words[] = {"dog",  "ant",     "cat", "man",    "bear",
                   "frog", "leopard", "bat", "rabbit", "monkey"};
  int to_do;

  if (TEST)
    printf("Wpisz nr zadania ");
  scanf("%d", &to_do);

  switch (to_do) {
  case 1: // Quick sort
    quick_sort(a, n);
    for (int i = 0; i < n; i++) {
      printf("%d ", a[i]);
    }
    break;
  case 2: // Sortowanie słów

    sort_words(words, n);

    for (int i = 0; i < n; i++) {
      printf("%s ", words[i]);
    }
    if (TEST)
      printf("\n");
    break;
  case 3: // Sortowanie leksykograficzne struktur
    s[0].name = "dog";
    s[0].nazwa = "pies";
    s[1].name = "ant";
    s[1].nazwa = "mrowka";
    s[2].name = "cat";
    s[2].nazwa = "kot";
    s[3].name = "Man";
    s[3].nazwa = "czlowiek";
    s[4].name = "bear";
    s[4].nazwa = "niedzwiedz";
    s[5].name = "frog";
    s[5].nazwa = "zaba";
    s[6].name = "leopard";
    s[6].nazwa = "lampart";
    s[7].name = "BAT";
    s[7].nazwa = "nietoperz";
    s[8].name = "rabbit";
    s[8].nazwa = "krolik";
    s[9].name = "monkey";
    s[9].nazwa = "malpa";
    struct_lexi_sort(&n);
    for (int i = 0; i < n; i++) {
      printf("%s ", s[i].name);
      if (TEST)
        printf("znaczy ");
      printf("%s ", s[i].nazwa);
      if (TEST)
        printf("\n");
    } // ODKOMENTUJ W ZADANIU 3
    break;
  case 4: // Złożone wymagania dotyczące sortowania
    n++;
    sa[0].sname = "Anastazja";
    sa[0].fname = "Kamiska";
    sa[0].age = 28;
    sa[1].sname = "Stefan";
    sa[1].fname = "Kowal";
    sa[1].age = 36;
    sa[2].sname = "Kamila";
    sa[2].fname = "Munk";
    sa[2].age = 25;
    sa[3].sname = "Ernest";
    sa[3].fname = "Goja";
    sa[3].age = 27;
    sa[4].sname = "Karol";
    sa[4].fname = "Wodecki";
    sa[4].age = 27;
    sa[5].sname = "Anna";
    sa[5].fname = "Stone";
    sa[5].age = 15;
    sa[6].sname = "Katarzyna";
    sa[6].fname = "Michalak";
    sa[6].age = 20;
    sa[7].sname = "Małgorzata";
    sa[7].fname = "Kowal";
    sa[7].age = 33;
    sa[8].sname = "Emil";
    sa[8].fname = "Kant";
    sa[8].age = 22;
    sa[9].sname = "Kamil";
    sa[9].fname = "Mist";
    sa[9].age = 29;
    sa[10].sname = "Tomasz";
    sa[10].fname = "Nod";
    sa[10].age = 18;
    struct_name_sort(&n);
    for (int i = 0; i < n; i++) {
      printf("%s %s %d ", sa[i].sname, sa[i].fname, sa[i].age);
      if (TEST)
        printf("\n");
    } // ODKOMENTUJ W ZADANIU 4
    break;
  default:
    printf("NOTHING TO DO FOR %d\n", to_do);
  }
  return 0;
}
