/* { dg-do compile } */
/* { dg-require-effective-target arm_mabi_ilp32 } */
/* { dg-options "-mcmodel=tiny -mabi=ilp32 -fPIC" } */

extern int bar (void *);
extern long long a;

int
foo (void)
{
  a = 1;
  return bar ((void *)bar);
}

