#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char card_type;
#define N_CARDS 52
#define ARR_LEN(A) (sizeof(A) / sizeof(A[0]))

// Utility
void swap(card_type *a, card_type *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

// Circular buffer stuff
struct cbuf {
  int len;
  int pos;
  card_type buf[N_CARDS];
};

int cbuf_normalize(int ix) {
  int buf_len = N_CARDS;
  if (ix >= buf_len)
    ix -= buf_len;
  if (ix < 0)
    ix += buf_len;
  return ix;
}

void cbuf_push(struct cbuf *B, card_type val) {
  if (B->len == N_CARDS) { // Should never happen
    abort();
  }
  int ix = cbuf_normalize(B->len + B->pos);
  B->buf[ix] = val;
  B->len++;
}

card_type cbuf_pop(struct cbuf *B) {
  if (B->len == 0) { // Should never happen
    abort();
  }
  card_type val = B->buf[B->pos];
  B->len--;
  B->pos = cbuf_normalize(B->pos + 1);
  return val;
}

card_type cbuf_peek(const struct cbuf *B, int ix) {
  ix = cbuf_normalize(B->pos + ix);
  return B->buf[ix];
}
void cbuf_print(const struct cbuf *B) {
  int ix = B->pos;
  for (int i = 0; i < B->len; i++) {
    printf("%d ", B->buf[ix]);
    ix = cbuf_normalize(ix + 1);
  }
}

// RAND stuff

int rand_from_interval(int a, int b) {
  if (a > b)
    return INT_MIN;
  int range = b - a;
  if (range > RAND_MAX)
    return INT_MAX;
  if (a == b)
    return a;
  return a + (rand() % (range + 1));
}

void rand_permutation(int n, card_type tab[]) {
  if (n < 0)
    return;
  for (int i = 0; i < n; i++) {
    tab[i] = i;
  }
  for (int i = 0; i < n - 1; i++) {
    int k = rand_from_interval(i, n - 1);
    swap(&tab[i], &tab[k]);
  }
}

// Card specific
int card_power(card_type card) {
  return card / 4; // will be optimized to (card >> 2) by the compiler
}

enum result { A_wins, B_wins, draw };
enum result fight(struct cbuf *A, struct cbuf *B, int depth) {
  int a = card_power(cbuf_peek(A, depth));
  int b = card_power(cbuf_peek(B, depth));
  if (a > b)
    return A_wins;
  if (a == b)
    return draw;
  return B_wins;
}

void give_cards(struct cbuf *from, struct cbuf *to, int count) {
  for (int i = 0; i < count; i++) {
    card_type card = cbuf_pop(from);
    cbuf_push(to, card);
  }
}

enum gamemode { gamemode_normal = 0, gamemode_simplified = 1 };

/* 0 - game unfinished
 * 1 - not enough cards to finish war
 * 2 - A wins
 * 3 - B wins
 */
enum game_result { out_of_moves, out_of_cards, A_win, B_win };

int moves = 0;
enum game_result play(struct cbuf *A, struct cbuf *B, int max_moves,
                      enum gamemode mode) {
  int cards_on_table = 0;
  //   int moves = 0;
  while (1) {
    if (++moves > max_moves)
      return out_of_moves;
    if (A->len == 0)
      return B_win; // B wins
    if (B->len == 0)
      return A_win; // A wins

    cards_on_table++;
    // Check if players have enough cards
    if (A->len < cards_on_table || B->len < cards_on_table)
      return out_of_cards;

    enum result res = fight(A, B, cards_on_table - 1);
    switch (res) {
    case A_wins: {
      give_cards(A, A, cards_on_table);
      give_cards(B, A, cards_on_table);
      cards_on_table = 0;
      break;
    }
    case B_wins: {
      give_cards(B, B, cards_on_table);
      give_cards(A, B, cards_on_table);
      cards_on_table = 0;
      break;
    }
    case draw: {
      if (mode == gamemode_normal) {
        cards_on_table += 1; // Put one card face down
      } else if (mode == gamemode_simplified) {
        give_cards(B, B, cards_on_table);
        give_cards(A, A, cards_on_table);
        cards_on_table = 0;
      }
      break;
    }
    }
  }
}

int main() {
  int seed;
  scanf("%d", &seed);
  srand(seed);

  enum gamemode mode;
  scanf("%hd", (short *)&mode);

  int max_conflicts;
  scanf("%d", &max_conflicts);

  card_type deque[N_CARDS];
  for (uint i = 0; i < ARR_LEN(deque); i++) {
    deque[i] = i;
  }
  // Shuffle cards
  rand_permutation(ARR_LEN(deque), deque);

  // Hand cards to players
  const int half_cards = N_CARDS / 2;
  struct cbuf A = {.len = half_cards, .pos = 0, .buf = {0}};
  struct cbuf B = A;
  memcpy(A.buf, deque, sizeof(deque) / 2);
  memcpy(B.buf, deque + half_cards, sizeof(deque) / 2);

  enum game_result result = play(&A, &B, max_conflicts, mode);

  printf("%d ", result);
  switch (result) {
  case out_of_moves:
  case out_of_cards: {
    printf("%d %d ", A.len, B.len);
    break;
  }
  case A_win: {
    printf("%d ", moves - 1);
    break;
  }
  case B_win: {
    cbuf_print(&B);
    break;
  }
  }
  putchar('\n');
}
