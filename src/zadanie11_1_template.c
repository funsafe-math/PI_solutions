#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR -1
#define FILE_OPEN_ERROR -2
#define LIST_ERROR -3
#define PROGRAM_ERROR -4
#ifndef TEST
#define TEST 0 // 1 dla testowania, 0 dla automatycznej oceny
#endif

typedef struct tagListElement {
  struct tagListElement *next;
  void *data;
} ListElement;

ListElement *create_list_element(void *data, ListElement *next) {
  ListElement *n = (ListElement *)malloc(sizeof(ListElement));
  n->data = data;
  n->next = next;
  return n;
}

typedef void (*ConstDataFp)(const void *);
typedef void (*DataFp)(void *);
typedef int (*CompareDataFp)(const void *, const void *);
typedef void (*InsertInOrder)(void *, void *);

typedef struct tagList {
  ListElement *head;
  ListElement *tail;
  int size;
  ConstDataFp dumpData;
  DataFp freeData;
  CompareDataFp compareData;
  InsertInOrder insertElem;
} List;

////////////////////////////////////////////////////////////////
// Funkcje obslugi listy

// wypisanie zawartosci elemntow listy
void dumpList(const List *list) {
  if (list->dumpData == NULL)
    return;
  ListElement *i;
  for (i = list->head; i != NULL; i = i->next)
    list->dumpData(i->data);
  putchar('\n');
}

// zwolnienie pamieci listy
void freeList(List *list) {
  ListElement *current = 0;
  current = list->head;
  while (current) {
    ListElement *todelete = current;
    current = current->next;
    if (list->freeData)
      list->freeData(todelete->data); // zwolnienie pamięci dla struktury data
    free(todelete);                   // zwolnienie pamięci dla elementu
  }
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
}

///////////////////////////////////////////////////////////

// Funkcje obslugi elementow listy

/////////////////// Do zadania 11.1.1 i nastepnych:

// dodaje element na poczatku listy
void pushFront(List *list, void *data) {
  ListElement *element = (ListElement *)malloc(sizeof(ListElement));
  if (element == NULL)
    exit(MEMORY_ALLOCATION_ERROR);
  element->data = data;
  element->next = list->head;
  list->head = element;
  if (!list->tail)
    list->tail = list->head;
  list->size++;
}

// dodaje element na koncu listy
void pushBack(List *list, void *data) {
  ListElement *element = (ListElement *)malloc(sizeof(ListElement));
  if (element == NULL)
    exit(MEMORY_ALLOCATION_ERROR);
  element->data = data;
  element->next = NULL;
  if (list->tail)
    list->tail->next = element;
  list->tail = element;
  if (!list->head)
    list->head = list->tail;
  list->size++;
}

// usun pierwszy element listy
void popFront(List *list) {
  void *data;
  if (list->head) {
    ListElement *todelete = list->head;
    if (list->tail == todelete)
      list->tail = NULL;
    list->head = list->head->next;
    if (list->freeData)
      list->freeData(todelete->data);
    free(todelete);
    list->size--;
  }
}

// odwraca kolejnosc elementow listy
void reverse(List *list) {
  ListElement *a = list->head;
  if (a == NULL)
    return;
  ListElement *b = a->next;
  list->tail = a;
  a->next = NULL;
  while (b != NULL) {
    ListElement *tmp = b->next;
    b->next = a;
    a = b;
    b = tmp;
  }
  // a->next = NULL;
  list->head = a;
}

// wstawia element wg porzadku (okreslonym funkcja w naglowku listy w polu
// insertElem)
void insertInOrder(void *a, void *b) {
  List *plist = (List *)a;
  plist->insertElem(plist, b);
}

// Propozycje funkcji pomocniczych dla list uporzadkowanych

// zwraca element w posortowanej liscie, a jezeli nie ma, to element
// poprzedzajacy (nie ma, to NULL)
ListElement *findInsertionPoint(const List *list, ListElement *element) {
  ListElement *current = 0;
  ListElement *previous = 0;
  int c;
  for (current = list->head; current;
       previous = current, current = current->next) {
    c = list->compareData(current->data, element->data);
    if (c == 0)
      return current;
    if (c > 0)
      return previous;
  }
  return previous;
}

// dodaje element za wskazanym elementem - jezeli element NULL, to blad
void pushAfter(List *list, void *data, ListElement *previous) {
  /* Implementation not strictly required */
  if (previous == NULL)
    exit(PROGRAM_ERROR);
  ListElement *node = (ListElement *)malloc(sizeof(ListElement));
  node->data = data;
  node->next = previous->next;
  previous->next = node;
  list->size++;
}

/////////////////// Do zadania 11.1.2

// dodaje element na koncu listy bez korzystania z pola tail
void pushBack_v0(List *list, void *data) {
  ListElement *added = (ListElement *)malloc(sizeof(ListElement));
  added->next = NULL;
  added->data = data;

  ListElement *e = list->head;
  if (e == NULL) {
    list->head = added;
    list->tail = added;
    list->size = 1;
    return;
  }

  for (; e->next != NULL; e = e->next)
    ;
  e->next = added;

  list->size++;
  list->tail = added; // use tail to maintain valid state
}

// Funkcje warstwy obslugi danych wskazywanych w elementach listy

//////  Dla zadania 11.1.1 i 11.1.2
// 1. Dane: liczba typu int

typedef struct DataInt {
  int ID;
} DataInt;

void dump_int(const void *d) {
  const DataInt *data = (const DataInt *)d;
  printf("%d ", data->ID);
}

void free_int(void *d) {
  free(d);
}

int cmp_int(const void *a, const void *b) {
  DataInt *first = (DataInt *)a;
  DataInt *second = (DataInt *)b;
  return first->ID - second->ID;
}

DataInt *create_data_int(int v) {
  DataInt *p = (DataInt *)malloc(sizeof(DataInt));
  p->ID = v;
  return p;
}

// dodaje element do posortowanej listy, jezeli taki element juz istnieje, to
// NTD (Nothing To Do)
// void addList_int(List *plist, int v) {
void insertElem_int(List *list, DataInt *d) {
  if (list->size) {
    ListElement fakeroot = {list->head, NULL};
    ListElement *fast_ptr = list->head;
    ListElement *lag_ptr = &fakeroot;
    int cmp;
    while (fast_ptr && ((cmp = list->compareData(d, fast_ptr->data)) > 0)) {
      lag_ptr = fast_ptr;
      fast_ptr = fast_ptr->next;
    }
    if (cmp != 0) {
      lag_ptr->next = create_list_element(d, fast_ptr);
      list->head = fakeroot.next;
      if (fast_ptr == NULL)
        list->tail = lag_ptr->next;
      list->size++;
    }

  } else { // Add only list element
    list->head = create_list_element(d, NULL);
    list->tail = list->head;
    list->size = 1;
  }
}

// inicjowanie listy
void init_List_int(List *p_list) {
  p_list->head = NULL;
  p_list->tail = NULL;
  p_list->size = 0;
  p_list->dumpData = dump_int;
  p_list->freeData = free_int;
  p_list->compareData = cmp_int;
  p_list->insertElem = (InsertInOrder)insertElem_int;
}

//////  Dla zadania 11.1.3 i 11.1.4

// Dane: struktura zawierajaca pole wskaznika na lancuch znakow i ew. inne pola

typedef struct DataWord {
  char *word;
  int counter;
} DataWord;

DataWord *get_data_word(ListElement *e) {
  return (DataWord *)e->data;
}

void dump_word(const void *d) {
  const DataWord *word = (const DataWord *)d;
  // printf("(\"%s\", %d) ", word->word, word->counter);
  printf("%s ", word->word);
}

void free_word(void *d) {
  DataWord *word = (DataWord *)d;
  free(word->word);
  free(word);
}

int cmp_word_alphabet(const void *a, const void *b) {
  DataWord *first = (DataWord *)a;
  DataWord *second = (DataWord *)b;
  return strcmp(first->word, second->word);
}

int cmp_word_counter(const void *a, const void *b) {
  DataWord *first = (DataWord *)a;
  DataWord *second = (DataWord *)b;
  return first->counter - second->counter;
}

// wypisz dane elementow spelniajacych warunek: krotnosc == n
void dumpList_word_if(List *plist, int n) {
  ListElement *node = plist->head;
  if (node == NULL)
    return;
  for (; node != NULL; node = node->next) {
    const DataWord *data = (const DataWord *)node->data;
    // if (data->counter % n == 0)
    if (data->counter == n)
      dump_word(data);
  }
}

// przydziel pamiec dla danych
DataWord *create_data_word(char *string, int n) {
  DataWord *data = (DataWord *)malloc(sizeof(DataWord));
  data->word = strndup(string, n);
  data->counter = 1;
  return data;
}

void to_lowercase(char *s) {
  for (; *s; ++s)
    *s = tolower(*s);
}

// dodaje element do uporzadkowanej listy,
// jezeli element z danym wyrazem juz istnieje, to zwieksza licznik krotnosci
void insertElem_word(List *list, DataWord *d) {
  if (list->size) {
    ListElement fakeroot = {list->head, NULL};
    ListElement *fast_ptr = list->head;
    ListElement *lag_ptr = &fakeroot;
    int cmp;

    to_lowercase(d->word);
    while (fast_ptr && ((cmp = list->compareData(d, fast_ptr->data)) > 0)) {
      lag_ptr = fast_ptr;
      fast_ptr = fast_ptr->next;
    }
    if (cmp == 0) {
      DataWord *str = get_data_word(fast_ptr);
      str->counter++;
    } else {
      lag_ptr->next = create_list_element(d, fast_ptr);
      list->head = fakeroot.next;
      if (fast_ptr == NULL)
        list->tail = lag_ptr->next;
      list->size++;
    }

  } else {
    list->head = create_list_element(d, NULL);
    list->tail = list->head;
    list->size = 1;
  }
}

void init_List_word(List *p_list) {
  p_list->head = NULL;
  p_list->tail = NULL;
  p_list->size = 0;
  p_list->dumpData = dump_word;
  p_list->freeData = free_word;
  p_list->compareData = cmp_word_alphabet;
  p_list->insertElem = (InsertInOrder)insertElem_word;
}

//////////////////////////////////////////////////

/////////////////// Dla zadan 11.1.3 i 11.1.4

// Wyrazy ze strumienia wejsciowego zapisz w elementach listy
// w kolejnosci wskazanej przez order (0-wczytwywania, 1-alfabetycznej)
void stream_to_list(List *p_list, FILE *stream, int order) {
  const char *delimiters = ",.?!:;-\t\n\b\r ";
  char *line = NULL;
  ssize_t line_len = 0;
  size_t len;
  while ((line_len = getline(&line, &len, stream)) > 0) {
    char *token = NULL;
    for (token = strtok(line, delimiters); token != NULL;
         token = strtok(NULL, delimiters)) {
      DataWord *data = create_data_word(token, strlen(token));

      if (order) { // alphabetical order
        p_list->insertElem(p_list, data);
      } else { // load order
        pushBack(p_list, data);
      }
    }
    free(line);
    line = NULL;
  }
  free(line);
}

//
void list_test(List *plist, int n) {
  char op[2];
  int v;
  ListElement element, *pelement;
  DataInt data;
  for (int i = 0; i < n; ++i) {
    if (TEST)
      printf("Wpisz kod polecenia ");
    scanf("%s", op);
    switch (op[0]) {
    case 'f': // dodaj na poczatku listy
      if (TEST)
        printf("Wpisz wartosc ");
      scanf("%d", &v);
      pushFront(plist, create_data_int(v));
      break;
    case 'b': // dodaj na koncu listy
      if (TEST)
        printf("Wpisz wartosc ");
      scanf("%d", &v);
      pushBack(plist, create_data_int(v));
      break;
    case 'd': // usun pierwszy element
      popFront(plist);
      break;
    case 'r': // odwroc kolejnosc elementow listy
      reverse(plist);
      break;
    case 'i': // dodaj wg porzadku rosnacego (int) lub alfabetycznego (char),
              // jezeli element juz istnieje, to akcja zalezna od typu danych
      if (TEST)
        printf("Wpisz wartosc ");
      scanf("%d", &v);
      insertInOrder(plist, create_data_int(v));
      break;
    default:
      printf("No such operation: %s\n", op);
      break;
    }
    // printf("List : ");
    // dumpList(plist);
    // puts("");
  }
}

int main(void) {
  int to_do, n, size;
  char file_name[257];
  clock_t t_start, t_end;
  double t_total;
  FILE *stream;
  List list;
  ListElement *head;

  if (TEST)
    printf("Wpisz nr zadania ");
  scanf("%d", &to_do);
  switch (to_do) {
  case 1: {
    if (TEST)
      printf("Wpisz liczbe polecen ");
    scanf("%d", &n);
    head = NULL;
    init_List_int(&list);
    list_test(&list, n);
    dumpList(&list);
    freeList(&list);
    break;
  }
  case 2: {
    if (TEST)
      printf("Wpisz liczbe elementow ");
    scanf("%d", &size);
    init_List_int(&list);
    t_start = clock();
    for (int i = 0; i < size; ++i)
      pushBack_v0(&list, create_data_int(i));
    t_end = clock();
    t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("n = %d. Back bez tail.  Czas = %g s.\n", size, t_total);
    freeList(&list);

    init_List_int(&list);
    t_start = clock();
    for (int i = 0; i < size; ++i)
      pushFront(&list, create_data_int(i));
    reverse(&list);
    t_end = clock();
    t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("n = %d. Front + revers. Czas = %g s.\n", size, t_total);
    freeList(&list);

    init_List_int(&list);
    t_start = clock();
    for (int i = 0; i < size; ++i)
      pushBack(&list, create_data_int(i));
    t_end = clock();
    t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("n = %d. Back z tail.    Czas = %g s.\n", size, t_total);
    freeList(&list);
    break;
  }
  case 3: // wypisz wszystkie wyrazy wczytane do elementow listy
  {
    init_List_word(&list);
    if (TEST) {
      stream = stdin;
      printf("Wpisz linie tekstu\n");
      stream_to_list(&list, stream, 0);
      rewind(stream);
    } else {
      scanf("%256s", file_name);
      stream = fopen(file_name, "r");
      if (stream == NULL)
        exit(FILE_OPEN_ERROR);
      stream_to_list(&list, stream, 0);
      fclose(stream);
    }
    dumpList(&list);
    freeList(&list);
    break;
  }
  case 4: // wypisz w porzadku alfabetycznym wyrazy o zadanej krotnosci
  {
    if (TEST)
      printf("Wpisz krotnosc wyrazu ");
    scanf("%d", &n);
    init_List_word(&list);
    if (TEST) {
      stream = stdin;
      printf("Wpisz linie tekstu\n");
      stream_to_list(&list, stream, 1);
      rewind(stream);
    } else {
      scanf("%256s", file_name);
      stream = fopen(file_name, "r");
      if (stream == NULL)
        exit(FILE_OPEN_ERROR);
      stream_to_list(&list, stream, 1);
      fclose(stream);
    }
    list.compareData = cmp_word_counter;
    dumpList_word_if(&list, n);
    printf("\n");

    freeList(&list);
    break;
  }
  default: {
    printf("NOTHING TO DO FOR %d\n", to_do);
  }
  }
  return 0;
}
