#include <math.h>
#include <stdio.h>
#define N 100

#include "template/vec-template.c"

// generates the n-element sequence by incrementing the start value
// using the step size
void range(double v[], int n, double start, double step) {
  for (int i = 0; i < n; i++) {
    v[i] = start + step * i;
  }
}

// Returns n evenly spaced samples, calculated over the interval [start, stop].
// n >= 0
// for n = 0 return empty array
// for n = 1 return one-element array, with array[0] = start
void linspace(double v[], double start, double stop, int n) {
  double range = (stop - start) * n / (n - 1);
  v[0] = start;
  for (int i = 1; i < n; i++) {
    v[i] = start + range * i / n;
  }
}

// multiply each element of v by the value of scalar
void multiply_by_scalar(double v[], int n, double scalar) {
  for (int i = 0; i < n; i++) {
    v[i] *= scalar;
  }
}

// add to each element v1[i] value of v2[i]
void add(double v1[], const double v2[], int n) {
  for (int i = 0; i < n; i++) {
    v1[i] += v2[i];
  }
}

// calculate and return the dot product of v1 and v2
double dot_product(const double v1[], const double v2[], int n) {
  double res = 0.0;
  for (int i = 0; i < n; i++) {
    res += v1[i] * v2[i];
  }
  return res;
}

// read double vector of size n
void read_vector(double v[], int n) {
  for (int i = 0; i < n; i++) {
    scanf("%lf", v + i);
  }
}
