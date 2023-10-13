/* { dg-do compile } */
/* { dg-options "-O2" } */

#if __APPLE__
#define F128TYPE __float128
#else
#define F128TYPE long double
#endif

void
foo (F128TYPE *output)
{
  *output = 0.0;
}

/* { dg-final { scan-assembler "stp\\txzr, xzr, \\\[x0\\\]" } } */
