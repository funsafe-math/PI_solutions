#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR -1
#define FILE_OPEN_ERROR -2

#define TEST 0 // 1 dla testowania, 0 dla automatycznej oceny

//###################### deklaracja struktur
// utworzenie węzła dla teblicy
typedef struct Node {
  int *data;
  int len;
  struct Node *next;
  struct Node *prev;
} Node;

// utworzenie tagów listy
typedef struct tagList {
  Node *head;
  Node *tail;
  int size;
} List;

// utworzenie węzła dla teblicy znaków
typedef struct strNode {
  char first_letter;
  char **data;
  int len;
  struct strNode *next;
  struct strNode *prev;
} strNode;

strNode *create_strNode(strNode *prev, strNode *next, char *str) {
  strNode *n = (strNode *)malloc(sizeof(strNode));
  n->next = next;
  if (next)
    next->prev = n;
  n->prev = prev;
  if (prev)
    prev->next = n;
  n->len = 1;
  n->data = (char **)malloc(sizeof(char *) * n->len);
  n->data[0] = str;
  n->first_letter = str[0];
  return n;
}

// utworzenie tagów listy znaków
typedef struct strtagList {
  strNode *head;
  strNode *tail;
  int size;
} strList;

// utworzenie iteratora
typedef struct iterator {
  int arraypos;
  struct Node *nodenum;
} iterator;

// inicjalizacja iteratora do przodu
iterator initiate_forw(Node *head) {
  iterator it = {0, head};
  return it;
}

// inicjalizacja iteratora do tyłu
iterator initiate_back(Node *tail) {
  iterator it = {tail->len - 1, tail};
  return it;
}

/* inicjalizacja listy */
void init(List *list) {
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
}

/* inicjalizacja listy znaków*/
void sinit(strList *list) {
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
}

//###################### kod do uzupełnienia(zadania)

//### zadanie 1
// wstawienie elementu na końcu listy dwukierunkowej
void push_back(List *list, int *data, int len) {
  Node *n = (Node *)malloc(sizeof(Node));
  n->data = data;
  n->len = len;
  n->next = NULL;
  n->prev = NULL;
  list->size++;
  if (list->size == 1) {
    list->head = n;
    list->tail = n;
    return;
  }
  list->tail->next = n;
  n->prev = list->tail;
  list->tail = n;
}

//### zadanie 2
// funkcja przesuwająca iterator do przodu
void skip_next(iterator *it, int n) {
  n += it->arraypos;
  while (n >= it->nodenum->len) {
    n -= it->nodenum->len;
    it->nodenum = it->nodenum->next;
    if (it->nodenum == NULL)
      abort();
  }
  it->arraypos = n;
}

// iteracja do przodu
int get_forw(List *list, int num) {
  num -= 1; // Indexing starts from 1
  iterator it = initiate_forw(list->head);
  skip_next(&it, num);
  return it.nodenum->data[it.arraypos];
}

//### zadanie 3
// funkcja przesuwająca iterator do tyłu
void skip_prev(iterator *it, int n) {
  while (n > it->nodenum->len) {
    n -= it->nodenum->len;
    it->nodenum = it->nodenum->prev;
    if (it->nodenum == NULL)
      abort();
  }
  it->arraypos = it->nodenum->len - n;
}

// iteracja do tyłu
int get_back(List *list, int num) {
  iterator it = initiate_back(list->tail);
  skip_prev(&it, num);
  return it.nodenum->data[it.arraypos];
}

void remove_node(List *list, Node *node) {
  list->size--;
  if (node->prev)
    node->prev->next = node->next;
  if (node->next)
    node->next->prev = node->prev;
  free(node->data);
  free(node);
}

//### zadanie 4
// usuwanie elementu, jeżeli jest to ostatni element w tablicy to usuwa element
// listy
void remove_at(List *list, int num) {
  iterator it = initiate_forw(list->head);
  skip_next(&it, num - 1);
  if (it.nodenum->len == 1) {
    remove_node(list, it.nodenum);
    return;
  }
  for (int i = it.arraypos; i < it.nodenum->len - 1; i++) {
    it.nodenum->data[i] = it.nodenum->data[i + 1];
  }
  it.nodenum->len--;
}

int str_comparator(const void *a, const void *b) {
  const char *first = *(const char **)a;
  const char *second = *(const char **)b;
  return strcmp(first, second);
}

//### zadanie 5
// wstawienie słowa do komórki
void put_word_to_cell(strList *list, char *data) {
  char char_max_str[] = {
      (char)CHAR_MAX,
  };
  if (list->size) {
    strNode *fakeBegin = create_strNode(NULL, list->head, "");
    strNode *fakeEnd = create_strNode(list->tail, NULL, char_max_str);
    {
      strNode *n = fakeBegin;
      while (n != NULL) {
        if (n->first_letter == data[0]) {
          size_t new_size = sizeof(char *) * (n->len + 1);
          n->data = (char **)realloc(n->data, new_size);
          n->data[n->len] = strdup(data);
          n->len++;
          qsort(n->data, n->len, sizeof(char *), str_comparator);
          break;
        } else if (n->first_letter < data[0] &&
                   data[0] < n->next->first_letter) {
          create_strNode(n, n->next, strdup(data));
          list->size++;
          break;
        }
        n = n->next;
      }
    }
    list->head = fakeBegin->next;
    list->tail = fakeEnd->prev;
    list->head->prev = NULL;
    list->tail->next = NULL;
    free(fakeBegin);
    free(fakeEnd);
  } else {
    strNode *node = create_strNode(NULL, NULL, strdup(data));
    list->head = node;
    list->tail = node;
    list->size = 1;
  }
}

//###################### funkcje pomocnicze

/* Wypisanie zawartości listy, iteracja po elementach listy */
void dumpList(const List *list) {
  Node *i;
  for (i = list->head; i != NULL; i = i->next) {
    printf("-> ");
    for (int k = 0; k < i->len; k++) {
      if (k < i->len - 1) {
        printf("%d, ", i->data[k]);
      } else {
        printf("%d ", i->data[k]);
      }
    }
  }
  printf("\n");
}

/* Wypisanie zawartości listy, iteracja po elementach listy */
void sdumpList(const strList *list) {
  strNode *i;
  for (i = list->head; i != NULL; i = i->next) {
    printf("-> ");
    for (int k = 0; k < i->len; k++) {
      if (k < i->len - 1) {
        printf("%s, ", i->data[k]);
      } else {
        printf("%s ", i->data[k]);
      }
    }
  }
  printf("\n");
}

/* Usuwanie pierwszego elementu */
void delete_front(List *list) {
  Node *toDelete;
  if (list->head == NULL)
    return;
  toDelete = list->head;
  list->head = list->head->next;
  if (list->head == NULL)
    list->tail = NULL;
  free(toDelete);
  list->size--;
}

/* Usuwanie pierwszego elementu str */
void strdelete_front(strList *list) {
  strNode *toDelete;
  if (list->head == NULL)
    return;
  toDelete = list->head;
  list->head = list->head->next;
  if (list->head == NULL)
    list->tail = NULL;
  for (int i = 0; i < toDelete->len; i++) {
    free(toDelete->data[i]);
  }
  free(toDelete->data);
  free(toDelete);
  list->size--;
}

void stream_to_listoftables(strList *list, FILE *stream) {
  char *p, buf[BUFFER_SIZE];
  char delimits[] = " \r\t\n.,?!-";
  while (fgets(buf, BUFFER_SIZE, stream)) {
    p = strtok(buf, delimits);
    while (p) {
      put_word_to_cell(list, p);
      p = strtok(NULL, delimits);
    }
  }
}

/* Zwalnianie całej listy */
void freeList(List *list) {
  while (list->head) {
    delete_front(list);
  }
}

/* Zwalnianie całej listy str*/
void strfreeList(strList *list) {
  while (list->head) {
    strdelete_front(list);
  }
}

// odczyt wektora wartości całkowitych
void read_vector(int tab[], int len) {
  for (int i = 0; i < len; ++i) {
    scanf("%d", tab + i);
  }
}

int main() {
  int to_do, n, len, tab[100];
  FILE *stream;

  List list;
  strList slist;

  if (TEST)
    printf("Wpisz numer testu [1, 5]: ");
  scanf("%d", &to_do);

  switch (to_do) {
  case 1:
    init(&list);

    if (TEST)
      printf("wprowadź liczbę węzłów: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      if (TEST)
        printf("podaj długość tablicy %d: ", i + 1);
      scanf("%d", &len);
      if (TEST)
        printf("podaj elementy tablicy %d: ", i + 1);
      int *tab = (int *)malloc(len * sizeof(int));
      read_vector(tab, len);
      push_back(&list, tab, len);
    }

    dumpList(&list);
    freeList(&list);
    break;
  case 2:
    init(&list);

    if (TEST)
      printf("wprowadź liczbę węzłów: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      if (TEST)
        printf("podaj długość tablicy %d: ", i + 1);
      scanf("%d", &len);
      if (TEST)
        printf("podaj elementy tablicy %d: ", i + 1);
      int *tab = (int *)malloc(len * sizeof(int));
      read_vector(tab, len);
      push_back(&list, tab, len);
    }
    if (TEST)
      dumpList(&list);

    if (TEST)
      printf("wprowadź liczbę elementów do znalezienia: ");
    scanf("%d", &len);
    if (TEST)
      printf("wprowadź numery wybranych elementów: ");
    read_vector(tab, len);
    for (int i = 0; i < len; i++) {
      printf("%d ", get_forw(&list, tab[i]));
    }

    freeList(&list);
    break;
  case 3:
    init(&list);

    if (TEST)
      printf("wprowadź liczbę węzłów: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      if (TEST)
        printf("podaj długość tablicy %d: ", i + 1);
      scanf("%d", &len);
      if (TEST)
        printf("podaj elementy tablicy %d: ", i + 1);
      int *tab = (int *)malloc(len * sizeof(int));
      read_vector(tab, len);
      push_back(&list, tab, len);
    }
    if (TEST)
      dumpList(&list);

    if (TEST)
      printf("wprowadź liczbę elementów do znalezienia: ");
    scanf("%d", &len);
    if (TEST)
      printf("wprowadź numery wybranych elementów: ");
    read_vector(tab, len);
    for (int i = 0; i < len; i++) {
      printf("%d ", get_back(&list, tab[i]));
    }

    freeList(&list);
    break;
  case 4:
    init(&list);

    if (TEST)
      printf("wprowadź liczbę węzłów: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      if (TEST)
        printf("podaj długość tablicy %d: ", i + 1);
      scanf("%d", &len);
      if (TEST)
        printf("podaj elementy tablicy %d: ", i + 1);
      int *tab = (int *)malloc(len * sizeof(int));
      read_vector(tab, len);
      push_back(&list, tab, len);
    }
    if (TEST)
      dumpList(&list);

    if (TEST)
      printf("wprowadź liczbę komórek do usunięcia: ");
    scanf("%d", &len);
    if (TEST)
      printf("wprowadź numery komórek do usunięcia: ");
    read_vector(tab, len);
    for (int i = 0; i < len; i++) {
      remove_at(&list, tab[i]);
    }

    dumpList(&list);
    freeList(&list);

    break;
  case 5:
    sinit(&slist);

    if (TEST) {
      stream = stdin;
      printf("Wpisz linie tekstu\n");
      stream_to_listoftables(&slist, stream);
      rewind(stream);
    } else {
      stream = fopen("Tekst_1b.txt", "r");
      if (stream == NULL)
        exit(FILE_OPEN_ERROR);
      stream_to_listoftables(&slist, stream);
      fclose(stream);
    }

    sdumpList(&slist);
    strfreeList(&slist);
    break;
  default:
    printf("Numer testu spoza zakresu [1, 5] %d", to_do);
    break;
  }
  return 0;
}