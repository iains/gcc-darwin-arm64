/* { dg-require-effective-target arm_mabi_ilp32 } */
/* { dg-options "-O2 -mabi=ilp32 -Wno-deprecated -fdata-sections" } */

int a[128];
long long *p;
void f() {
  for (long i = 0; i < sizeof(long); i++)
    p[i] = a[i];
}
