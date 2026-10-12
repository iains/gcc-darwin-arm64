/* PR target/100056 */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not {\t[us]bfiz\tw[0-9]+, w[0-9]+, 11} { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\t[us]bfiz\tw[0-9]+, w[0-9]+, 11} 2 { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler-times {\tadd\tw[0-9]+, w[0-9]+, w[0-9]+, uxtb\n} 2 { target { ! *-*-darwin* } } } } */
/* darwinpcs promotes the narrow argument in the caller, so one of the two
   extensions is already done on entry.  */
/* { dg-final { scan-assembler-times {\t[us]bfiz\tw[0-9]+, w[0-9]+, 11} 1 { target *-*-darwin* } } } */
/* { dg-final { scan-assembler-times {\tadd\tw[0-9]+, w[0-9]+, w[0-9]+, uxtb\n} 1 { target *-*-darwin* } } } */

int
or_shift_u8 (unsigned char i)
{
  return i | (i << 11);
}

int
or_shift_u3a (unsigned i)
{
  i &= 7;
  return i | (i << 11);
}

int
or_shift_u3b (unsigned i)
{
  i = (i << 29) >> 29;
  return i | (i << 11);
}

int
or_shift_s16 (signed short i)
{
  return i | (i << 11);
}

int
or_shift_s8 (signed char i)
{
  return i | (i << 11);
}

int
or_shift_s13 (int i)
{
  i = (i << 19) >> 19;
  return i | (i << 11);
}

int
or_shift_s3 (int i)
{
  i = (i << 29) >> 29;
  return i | (i << 11);
}

int
or_shift_u8_asm (unsigned char x)
{
  unsigned char i = x;
  asm volatile ("" : "+r" (i));
  return i | (i << 11);
}
