/* { dg-do compile } */
/* { dg-additional-options "-march=armv8.4-a+dotprod" } */

int main ()
{
  return 0;
}

/* { dg-final { scan-assembler-times {\.arch armv8\.4\-a\+crc\n} 1 { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler-times {\.arch armv8\.4\-a} 1 { target *-*-darwin* } } } */
/* { dg-final { scan-assembler-not {\+dotprod} } } */

/* Checking if enabling default features drops the superfluous bits.   */
