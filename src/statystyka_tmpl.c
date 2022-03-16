#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define TEST 0 // 1 - ulatwia testowanie programu,
// 0 - dla wersji przeznaczonej do automatycznej oceny

// Zwraca wartość średniej arytmetycznej oraz wariancji elemntów tablicy o
// długości n. dla n = 0 zwracana jest pusta tablica dla n = 1 zwracana jest
// tablica jednoelementowa
void aver_varian(double v[], double *arith_aver, double *varian, int n) {
  *arith_aver = 0;
  for (int i = 0; i < n; i++) {
    *arith_aver += (double)v[i] / n;
  }
  double var_squared = 0;
  for (int i = 0; i < n; i++) {
    double diff = (v[i] - *arith_aver);
    var_squared += diff * diff;
  }
  var_squared /= (double)n;
  *varian = var_squared;
}

// Inclusive [a,b]
int get_rand(int a, int b) { return a + (rand() % (b - a + 1)); }

// Wypełnianie tablicy wynikiem n prób Bernoulliego
// 1 - orzeł; 0 - reszka
// Losowanie realizowane na 100-elemntowym zbiorze liczb całkowitych
void bernoulli_gen(int v[], int bernoulli_prop, int n) {
  if (n < 0)
    abort();
  for (int i = 0; i < n; i++) {
    int k = get_rand(0, 99);
    v[i] = k > bernoulli_prop;
  }
}

// Dyskretny rozkład prawdopodobieństwa reprezentujący procentowy udział
// poszczególnych wartości zmiennej losowej liczba elementów biorących udział w
// losowaniu to 11 bo na 2 kostkach możemy uzyskać liczby [2-12] trials_num -
// liczba losowań

void probab_distrib(double v[], int trials_num) {
  const int offset = 2;
  const int max_score = 12;
  const int min_score = 2;
  const int n_scores = max_score - min_score + 1;
  int counts[11] = {
      0,
  };
  for (int i = 0; i < trials_num; i++) {
    int a = get_rand(1, 6);
    int b = get_rand(1, 6);
    counts[a + b - offset]++;
  }
  for (int i = 0; i < n_scores; i++) {
    v[i] = (double)counts[i] * 100 / trials_num;
  }
}

// Oblicza i zwraca Dystrybuantę (Cumulative Distribution Function)
// len - liczba wylosowanych elementów z przediału
void var_dystryb(double dystr[], int len) {
  int counts[11] = {
      0,
  };
  for (int l = 0; l < len; l++) {
    int a = rand() % 6;
    int b = rand() % 6;
    int s = a + b;
    for (int i = s; i < 11; i++) {
      counts[i]++;
    }
  }
  for (int i = 0; i < 11; i++) {
    dystr[i] = (double)counts[i] * 100 / len;
  }
}

// Symuluje problem Monty'ego-Hall'a dla przypadku 3-bramokowego
// Wejście: n - liczba prób
// Wyjście: switch_wins - liczba wygranych przez zamianę, switch_probabil -
// prawdopodobieństwo wygranej przez zamianę Wyjście: non_switch_wins - liczba
// wygranych bez zamiany, non_switch_probabil - prawdopodobieństwo wygranej bez
// zamiany

void monty_hall(int *switch_wins, int *nonswitch_wins, double *switch_probabil,
                double *nonswitch_probabil, int n) {
  for (int i = 0; i < n; i++) {
    // Idiotic meta rand generator
    int winner = rand() % 2 ? -1 : 0;
    if (winner == -1) {
      winner = 1 + (rand() % 2);
    }
    // End idiotic meta rand generator
    *nonswitch_wins += (winner == rand() % 3);

    // Better solution:
    // *nonswitch_wins += (rand() % 3 == rand() % 3);
    // Order of evaluation does not matter!
  }
  *switch_wins = n - *nonswitch_wins;
  *nonswitch_probabil = (double)100 * *nonswitch_wins / n;
  *switch_probabil = (double)100 * *switch_wins / n;
}

// read double vector of size n
void read_vector(double v[], int n) {
  for (int i = 0; i < n; ++i) {
    scanf("%lf", v + i);
  }
}

// read int vector of size n
void read_int_vector(int v[], int n) {
  for (int i = 0; i < n; ++i) {
    scanf("%d", v + i);
  }
}

// print double vector of size n (with 2 figures after the decimal point)
void print_vector(const double v[], int n) {
  for (int i = 0; i < n; ++i) {
    printf("%.2f ", v[i]);
  }
  printf("\n");
}

// print integer vector
void print_bern_vector(int bern_v[], int n) {
  for (int i = 0; i < n; ++i) {
    printf("%d ", bern_v[i]);
  }
  printf("\n");
}

int main(void) {
  int to_do, len, seed, propabl, number, m_h_wins, non_m_h_wins, itab[100];
  double arith_aver, varian, m_h_probabil, non_m_h_probabil, dtab[100];
  if (TEST)
    printf("Wpisz numer testu [1, 5]: ");
  scanf("%d", &to_do);
  if (to_do > 1) {
    printf("seed = ");
    scanf("%d", &seed);
    srand(seed); // ustawienie ziarna dla powterzalnosci wynikow
  }

  switch (to_do) {
  case 1: // aver_varian
    arith_aver = 0.0;
    varian = 0.0;
    if (TEST)
      printf("aver_varian: Wpisz dlugosc wektora: ");
    scanf("%d", &len);
    if (len < 0 || len > 100) {
      printf("n<0 lub n > 100\n");
      break;
    }
    if (TEST)
      printf("Wpisz elementy wektora: ");
    read_vector(dtab, len);
    aver_varian(dtab, &arith_aver, &varian, len);
    printf("średnia arith_aver = %.2f wariancja = %.2f\n", arith_aver, varian);
    break;
  case 2: // bernoulli_gen
    if (TEST)
      printf("BERNOULLI_GEN: Wpisz dlugość wektora: ");
    scanf("%d", &len);
    if (len < 0 || len > 100) {
      printf("n<0 lub n > 100\n");
      break;
    }
    if (TEST)
      printf("Wpisz prawdopodobieństwo reszki w %%: ");
    scanf("%d", &propabl);
    bernoulli_gen(itab, propabl, len);
    print_bern_vector(itab, len);
    break;
  case 3: // probab_distrib
    if (TEST)
      printf("PROBAB_DISTRIB: Wpisz liczbę prób: ");
    scanf("%d", &len);
    if (len < 0 || len > 1000000) {
      printf("n < 0 lub n > 1000000\n");
      break;
    }
    probab_distrib(dtab, len);
    print_vector(dtab, 11); //
    break;
  case 4: // var_dystryb
    if (TEST)
      printf("VAR_DYSTRYB: Wpisz liczbę rzutów dwiema kostkami: ");
    scanf("%d", &len);
    if (len < 0 || len > 1000000) {
      printf("n<0 lub n > 1000000\n");
      break;
    }
    printf("Wpisz elementy wektora: ");
    var_dystryb(dtab, len);
    print_vector(dtab, 11); // 11 ponieważ w przypadku rzutu 2 kostkami
                            // istnieje 11 wartości zmiennej losowej [2-12]
    break;
  case 5: // monty_hall
    m_h_wins = non_m_h_wins = 0;
    m_h_probabil = non_m_h_probabil = 0.0;
    if (TEST)
      printf("MONTY_HALL: Wpisz liczbę prób: ");
    scanf("%d", &len);
    if (len < 0 || len > 1000000) {
      printf("n<0 lub n > 1000000\n");
      break;
    }
    monty_hall(&m_h_wins, &non_m_h_wins, &m_h_probabil, &non_m_h_probabil, len);
    if (TEST)
      printf("Po %d grach, wygrałem ", len);
    printf("%d ", m_h_wins);
    if (TEST)
      printf("razy przez zmianę wyboru. To jest [%%] ");
    printf("%.2f\n", m_h_probabil);
    if (TEST)
      printf("Natomiast, nie zmieniając wyboru wygrałem ");
    printf("%d ", non_m_h_wins);
    if (TEST)
      printf("razy. To jest [%%] ");
    printf("%.2f\n", non_m_h_probabil);
    break;
  default:
    printf("Numer testu spoza zakresu [1, 5] %d", to_do);
    break;
  }
  return 0;
}