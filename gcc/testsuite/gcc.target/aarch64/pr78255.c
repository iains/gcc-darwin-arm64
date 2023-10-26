/* { dg-do compile } */
/* { dg-skip-if "Darwin platforms do not support tiny" *-*-darwin* } */
/* { dg-options "-O2 -mcmodel=tiny" } */

extern int bar (void *);

int
foo (void)
{
  return bar ((void *)bar);
}

/* { dg-final { scan-assembler "b\\s+bar" } } */
