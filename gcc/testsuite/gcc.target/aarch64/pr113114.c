/* { dg-do compile } */
/* { dg-options "-mabi=ilp32 -O -mearly-ldp-fusion -mlate-ldp-fusion" } */
/* { dg-skip-if "-mabi=ilp32 unsupported" { aarch64-*-darwin* } } */
void foo_n(double *a) {
  int i = 1;
  for (; i < (int)foo_n; i++)
    a[i] = a[i - 1] + a[i + 1] * a[i];
}
