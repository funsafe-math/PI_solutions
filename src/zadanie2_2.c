#include <math.h>
#include <stdio.h>

#define STACK_SIZE 10

#define TEST 0 // 1 dla testowania, 0 dla oceny automatycznej

double stack[STACK_SIZE];
int top = 0; // jawne zerowanie nie jest konieczne
// top stores the size of stack

double stack_push(double x) {
  if (top == STACK_SIZE) {
    return INFINITY;
  }
  stack[++top - 1] = x;
  return 0;
}

double stack_pop(void) {
  if (top == 0)
    return NAN;
  return stack[--top];
}

int stack_state(void) {
  if (top == STACK_SIZE)
    return -1;
  return top;
}

// Kolejka z przesuwaniem w tablicy

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0; // 1. klient dostanie nr = 1

double queue_push(int in_nr) {
  int tmp_counter = curr_nr;
  curr_nr += in_nr;
  for (int i = 0; i < in_nr; i++) {
    if (in >= QUEUE_SIZE) {
      return INFINITY;
    }
    queue[in++] = ++tmp_counter;
  }
  return 0;
}

int queue_pop(int out_nr) {
  if (out_nr > in) {
    in = 0;
    return -1;
  }
  in -= out_nr;
  for (int i = 0; i < in; i++) {
    queue[i] = queue[i + out_nr];
  }
  return in;
}

void queue_state(void) {
  if (in == 0)
    printf("%f", NAN);
  for (int i = 0; i < in; i++) {
    printf("%d ", queue[i]);
  }
}

// Kolejka w buforze cyklicznym

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;

int cbuff_normalize_ix(int ix) {
  if (ix >= CBUFF_SIZE)
    ix -= CBUFF_SIZE;
  if (ix < 0)
    ix += CBUFF_SIZE;
  return ix;
}

double cbuff_push(int cli_nr) {
  if (len >= CBUFF_SIZE)
    return INFINITY;
  int ix = cbuff_normalize_ix(out + len);
  cbuff[ix] = cli_nr;
  len++;
  return 0;
}

int cbuff_pop(void) {
  if (len == 0)
    return -1;
  len -= 1;
  int val = cbuff[out];
  out = cbuff_normalize_ix(out + 1);
  return val;
}

void cbuff_state(void) {
  if (len == 0) {
    printf("%f", NAN);
    return;
  }
  for (int i = 0; i < len; i++) {
    printf("%d ", cbuff[out]);
    out = cbuff_normalize_ix(out + 1);
  }
}

int main(void) {
  int to_do, n, ans;
  int nr_klienta, kod_operacji;
  double x, f_ans;
  if (TEST)
    printf("Nr testu = ");
  scanf("%d", &to_do);
  switch (to_do) {
  case 1:
    if (TEST)
      printf("sekwencja operacji (w jednej linii, oddzielone spacjami):\n");
    // 0 podaje stan stosu i konczy sekwencje operacji
    do {
      scanf("%lf", &x);
      if (x > 0) {
        f_ans = stack_push(x);
        if (f_ans == INFINITY)
          printf("%f ", INFINITY);
      } else {
        if (x < 0)
          printf("%.2f ", stack_pop());
        else
          printf("\n%d\n", stack_state());
      }
    } while (x != 0.0);
    break;
  case 2:
    //  kolejka kolejnych liczb naturlnych > 0 (naplywajacych grupowo) -
    //  implementacja w tablicy z przesunieciami
    if (TEST)
      printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");
    // 0 podaje stan kolejki i konczy symulacje kolejki
    do {
      scanf("%d", &n);
      if (n > 0) {
        f_ans = queue_push(n);
        if (f_ans == INFINITY)
          printf("%f ", INFINITY);
      } else if (n < 0) {
        ans = queue_pop(-n);
        if (ans < 0)
          printf("%f ", NAN);
      } else
        queue_state();
    } while (n != 0);
    break;
  case 3:
    //  kolejka kolejnych liczb naturalnych > 0 (naplywajacych pojedynczo -
    //  implementacja w buforze cyklicznym
    if (TEST)
      printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");
    // 0 podaje stan kolejki i konczy symulacje kolejki
    nr_klienta = 0;
    do {
      scanf("%d", &kod_operacji);
      if (kod_operacji > 0) {
        f_ans = cbuff_push(++nr_klienta);
        if (f_ans == INFINITY)
          printf("%f ", INFINITY);
      } else if (kod_operacji < 0) {
        ans = cbuff_pop();
        if (ans < 0)
          printf("%f ", NAN);
        else
          printf("%d ", ans);
      } else
        cbuff_state();
    } while (kod_operacji != 0);
    break;
  default:
    if (TEST)
      printf("NOTHING TO DO!\n");
  }
  return 0;
}
