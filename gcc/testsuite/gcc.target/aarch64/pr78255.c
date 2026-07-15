/* { dg-do compile } */
/* { dg-options "-O2 -mcmodel=tiny" } */
/* { dg-require-effective-target aarch64_tiny_nocache } */

extern int bar (void *);

int
foo (void)
{
  return bar ((void *)bar);
}

/* { dg-final { scan-assembler "b\\s+bar" } } */
