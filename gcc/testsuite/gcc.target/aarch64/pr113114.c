/* { dg-do compile } */
/* { dg-skip-if "-mabi=ilp32 unsupported" { aarch64-*-darwin* } } */
/* { dg-options "-mabi=ilp32 -Wno-deprecated -O -mearly-ldp-fusion -mlate-ldp-fusion" } */

void foo_n(double *a) {
  int i = 1;
  for (; i < (int)foo_n; i++)
    a[i] = a[i - 1] + a[i + 1] * a[i];
}
