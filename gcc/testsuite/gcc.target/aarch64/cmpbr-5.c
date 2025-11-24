/* { dg-do compile } */
/* { dg-require-effective-target aarch64_asm_cmpbr_ok } */
/* { dg-options "-fgimple -O1" } */
#pragma GCC target "+cmpbr"
typedef unsigned short us;
__GIMPLE double
f (us x, us y, double a, double b)
{
  bool c;
  double d;
  c = x == y;
  d = c ? a : b;
  return d;
}
