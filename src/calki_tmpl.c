#include <math.h>
#include <stdio.h>
#ifndef TEST
#define TEST 0 // 1 - ulatwia testowanie programu,
#endif
// 0 - dla wersji przeznaczonej do automatycznej oceny

// Początek zadania 6.1 /////////////////////

double gen_x(double a, double b, int n, int i) {
  double range = b - a;
  return a + ((double)range * i / n);
}

double int_leftrect(double x1, double x2, int n, double (*func)(double)) {
  double dx = (x2 - x1) / n;
  double sum = 0.0;
  for (int i = 0; i < n; i++) {
    sum += func(gen_x(x1, x2, n, i));
  }
  return dx * sum;
}
// Koniec zadania 6.1 /////////////////////

// Początek zadania 6.2 /////////////////////
double int_rightrect(double x1, double x2, int n, double (*func)(double)) {
  double dx = (x2 - x1) / n;
  double sum = 0.0;

  for (int i = 0; i < n; i++) {
    sum += func(gen_x(x1, x2, n, i) + dx);
  }
  return dx * sum;
}

double int_midrect(double x1, double x2, int n, double (*func)(double)) {
  double dx = (x2 - x1) / n;
  double sum = 0.0, x;
  for (int i = 0; i < n; i++) {
    sum += func(gen_x(x1, x2, n, i) + dx / 2);
  }
  return dx * sum;
}

double int_trapez(double x1, double x2, int n, double (*func)(double)) {
  double dx = (x2 - x1) / n;
  double sum = func(x1) + func(x2);
  int i;
  for (i = 1; i < n; i++)
    sum += 2.0 * func(x1 + i * dx);
  return dx * sum / 2.0;
}

typedef double (*obj_func)(double);
typedef double (*int_func)(double, double, int, obj_func);
// Koniec zadania 6.2/////////////////////

// Początek zadania 6.3 /////////////////////
double double_integr(double x1, double x2, int nx, double y1, double y2, int ny,
                     double (*func)(double, double)) {
  double dx = (x2 - x1) / nx;
  double dy = (y2 - y1) / ny;

  double sum = 0.0, x, y;

  for (int y = 0; y < ny; y++) {
    for (int x = 0; x < nx; x++) {
      sum += func(gen_x(x1, x2, nx, x), gen_x(y1, y2, ny, y));
    }
  }
  return dx * dy * sum;
}

typedef double (*obj_func_d)(double, double);
// Koniec zadania 6.3 /////////////////////

// Początek zadania 6.4 /////////////////////
double integr3D(double x1, double x2, int nx, double y1, double y2, int ny,
                double z1, double z2, int nz,
                double (*func)(double, double, double)) {
  double dx = (x2 - x1) / nx;
  double dy = (y2 - y1) / ny;
  double dz = (z2 - z1) / nz;

  double sum = 0.0;

  for (int z = 0; z < nz; z++) {
    for (int y = 0; y < ny; y++) {
      for (int x = 0; x < nx; x++) {
        sum += func(gen_x(x1, x2, nx, x), gen_x(y1, y2, ny, y),
                    gen_x(z1, z2, nz, z));
      }
    }
  }
  return dx * dy * dz * sum;
}

typedef double (*obj_func_t)(double, double, double);
// Koniec zadania 6.4 /////////////////////

// Początek zadania 6.5 /////////////////////
double doub_int_bound(double x1, double x2, int nx, double y1, double y2,
                      int ny, int (*boundary)(double, double),
                      double (*func)(double, double)) {
  double dx = (x2 - x1) / nx;
  double dy = (y2 - y1) / ny;

  double sum = 0.0;
  double x, y;

  for (y = y1; y <= (y2 - dy); y += dy) {
    for (x = x1; x <= x2 - dx; x += dx) {
      if (boundary(x, y) == 1)
        sum += func(x, y);
    }
  }
  return dx * dy * sum;
}

typedef double (*obj_func_b)(double, double);
typedef int (*obj_bound)(double, double);
// Koniec zadania 6.5 /////////////////////

// Funkcje matematyczne jednej zmiennej do zadań 6.1 i 6.2
double f1(double x) { return x; }

double f2(double x) { return x * x / 2.0; }

double f3(double x) { return 1.0 / x; }

double f4(double x) { return log10(x); }
//////////////////////////////////////////////////////////

// Funkcje matematyczne dwóch zmiennych do zadań 6.3 i 6.5
double f5(double x, double y) { return pow(pow(x, 4) + pow(y, 5), 0.5); }

double f6(double x, double y) { return 8 * sin(x) + pow(5 * cos(y), 2); }

double f7(double x, double y) { return 9 * sqrt(x + y) - 2 * sqrt(y); }
/////////////////////////////////////////////////////

// Funkcje matematyczne trzech zmiennych do zadania 6.4
double f8(double x, double y, double z) { return x + 3 * y + 5 * z; }

double f9(double x, double y, double z) {
  return 8 * sin(x) + pow(5 * cos(y), 2) + 2 * z;
}
/////////////////////////////////////////////////////

// Funkcje podcałkowe do zadania 6.5 //////////////////

double fb1(double x, double y) { return x * (y * y); }

double fb2(double x, double y) { return (x * x) + (y * y); }

double fb3(double x, double y) { return 1 / (pow(1 - x * x - y * y, 2)); }

/////////////////////////////////////////////////////

// Funkcje boundary do zadania 6.5 //////////////////

int b1(double x, double y) {
  if (x * x + y * y <= 4 && x >= 0)
    return 1;
  return 0;
}

int b2(double x, double y) {
  double tmpr = x * x + y * x - 2 * y;
  if (tmpr >= 1 || tmpr <= 0)
    return 1;
  return 0;
}

int b3(double x, double y) {
  double tmpr = x * x + y * y;
  if (tmpr <= x && tmpr <= y)
    return 1;
  return 0;
}

/////////////////////////////////////////////////////

int main(void) {
  int to_do, n, nx, ny, nz;
  double ic, x1, x2, y1, y2, z1, z2;
  if (TEST)
    printf("Wpisz numer testu [1, 5]: ");
  scanf("%d", &to_do);
  switch (to_do) {
  case 1: // 6.1
  {
    if (TEST)
      printf("Zad. 6.1 - Całka jednowymiarowa (wsk. do funkcji, met. prostokąt "
             "lewostr.)\n");
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków [x1 x2 n]: ");
    scanf("%lf %lf %d", &x1, &x2, &n);

    if (TEST == 1)
      printf("(x^2)/2: ");
    printf("%.2lf ", int_leftrect(x1, x2, n, f2));
    if (TEST == 1)
      printf("\n");

    break;
  }
  case 2: // 6.2
  {
    if (TEST)
      printf("Zad. 6.2 - Całki jednowymiarowe (wsk. do funkcji, met. "
             "prostokąt, trapez)\n");
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków [x1 x2 n]: ");
    scanf("%lf %lf %d", &x1, &x2, &n);

    int_func intf[] = {&int_rightrect, &int_midrect, &int_trapez};
    obj_func objf[] = {f1, f2, f3, f4};
    const char *int_func_names[] = {"rightrect", "midrect", "trapez"};
    const char *obj_func_names[] = {"x", "(x^2)/2", "1/x", "log10(x)"};

    for (int j = 0; j < 4; j++) {
      if (TEST == 1)
        printf("%10s --> \n", obj_func_names[j]);
      for (int i = 0; i < 3; i++) // Pętla wywołująca
      {
        ic = (*intf[i])(x1, x2, n, objf[j]);
        // wskazanie funkcji całkującej z parametrami
        // zawierającymi wskazanie funkcji do całkowania
        if (TEST == 1)
          printf("%10s: ", int_func_names[i]);
        printf("%.2lf ", ic);
        if (TEST == 1)
          printf("\n");
      }
    }
    break;
  }
  case 3: // 6.3
  {
    if (TEST)
      printf("Zad. 6.3 - Całka podwójna (wsk. do funkcji)\n");
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków x [x1 x2 nx]: ");
    scanf("%lf %lf %d", &x1, &x2, &nx);
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków y [y1 y2 ny]: ");
    scanf("%lf %lf %d", &y1, &y2, &ny);

    obj_func_d objfd[] = {f5, f6, f7};
    const char *obj_func_names_d[] = {"sqrt(x^4+y^5)", "8sin(x)+(5cos(y))^2",
                                      "9sqrt(x+y)-2sqrt(y)"};
    for (int i = 0; i < 3; ++i) {
      ic = double_integr(x1, x2, nx, y1, y2, ny, objfd[i]);
      if (TEST == 1)
        printf("%10s: ", obj_func_names_d[i]);
      printf("%.2lf ", ic);
      if (TEST == 1)
        printf("\n");
    }
    break;
  }
  case 4: // 6.4
  {
    if (TEST)
      printf("Zad. 6.4 - Całka potrójna (wsk. do funkcji)\n");
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków x [x1 x2 nx]: ");
    scanf("%lf %lf %d", &x1, &x2, &nx);
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków y [y1 y2 ny]: ");
    scanf("%lf %lf %d", &y1, &y2, &ny);
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków z [z1 z2 nz]: ");
    scanf("%lf %lf %d", &z1, &z2, &nz);

    obj_func_t objft[] = {f8, f9};
    const char *obj_func_names_t[] = {"x+3y+5z", "8sin(x)+(5cos(y))^2+2z"};
    for (int i = 0; i < 2; ++i) {
      ic = integr3D(x1, x2, nx, y1, y2, ny, z1, z2, nz, objft[i]);
      if (TEST == 1)
        printf("%10s: ", obj_func_names_t[i]);
      printf("%.2lf ", ic);
      if (TEST == 1)
        printf("\n");
    }
    break;
  }
  case 5: // 6.5
  {
    if (TEST)
      printf("Zad. 6.5 - Całka podwójna (struktury -> prosta budowa całki "
             "wielokrotnej)\n");
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków x [x1 x2 nx]: ");
    scanf("%lf %lf %d", &x1, &x2, &nx);
    if (TEST)
      printf("Wpisz zakres całkowania i liczba kroków y [y1 y2 ny]: ");
    scanf("%lf %lf %d", &y1, &y2, &ny);

    obj_func_b objfb[] = {fb1, fb2, fb3};
    obj_bound objb[] = {b1, b2, b3};
    const char *obj_func_names_b[] = {"xy^2", "x^2+y^2", "1/(1-x^2-y^2)^2"};
    const char *obj_bound_names_b[] = {"x^2+y^2<=4, x>=0", "1<=x^2+x*y-2y<=0",
                                       "x^2+y^2<=x, x^2+y^2<=y"};
    for (int i = 0; i < 3; ++i) {
      ic = doub_int_bound(x1, x2, nx, y1, y2, ny, objb[i], objfb[i]);
      if (TEST == 1)
        printf("%10s, gdy %10s: ", obj_func_names_b[i], obj_bound_names_b[i]);
      printf("%.2lf ", ic);
      if (TEST == 1)
        printf("\n");
    }
    break;
  }
  default:
    printf("Numer testu spoza zakresu [1, 5] %d", to_do);
    break;
  }
  return 0;
}