/* Verify zero initialization for floating point type automatic variables.  */
/* { dg-do compile } */
/* { dg-options "-ftrivial-auto-var-init=zero -fdump-rtl-expand" } */

#ifdef __APPLE__
# define TYPE _Float128
#else
# define TYPE long double
#endif

TYPE result;

TYPE foo()
{
  float temp1;
  double temp2;
  TYPE temp3;
  
  result = temp1 + temp2 + temp3;
  return result;
}

/* { dg-final { scan-rtl-dump-times "const_double\:SF 0\.0" 1 "expand" } } */
/* { dg-final { scan-rtl-dump-times "const_double\:DF 0\.0" 1 "expand" } } */
/* { dg-final { scan-rtl-dump-times "const_double\:TF 0\.0" 1 "expand" } } */
