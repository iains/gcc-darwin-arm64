/* { dg-skip-if "no 32b support" { aarch64-*-darwin* } } */
/* { dg-options "-O2 -mabi=ilp32 -fdata-sections" } */

int a[128];
long long *p;
void f() {
  for (long i = 0; i < sizeof(long); i++)
    p[i] = a[i];
}
