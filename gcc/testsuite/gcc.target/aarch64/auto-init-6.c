/* Verify pattern initialization for complex type automatic variables.  */
/* { dg-do compile } */
/* { dg-options "-ftrivial-auto-var-init=pattern" } */

#ifdef __APPLE__
# define TYPE _Float128
#else
# define TYPE long double
#endif

_Complex TYPE result;

_Complex TYPE foo()
{
  _Complex float temp1;
  _Complex double temp2;
  _Complex TYPE temp3;

  result = temp1 + temp2 + temp3;
  return result;
}

/* { dg-final { scan-assembler-times "word\t-16843010" 14  } } */
