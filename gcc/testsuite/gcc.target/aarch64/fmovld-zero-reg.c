/* { dg-do compile } */
/* { dg-options "-O2" } */

#if __APPLE__
#define F128TYPE __float128
#else
#define F128TYPE long double
#endif

void bar (F128TYPE);
void
foo (void)
{
  bar (0.0);
}

/* { dg-final { scan-assembler "movi\\tv0\.2d, #0" } } */
