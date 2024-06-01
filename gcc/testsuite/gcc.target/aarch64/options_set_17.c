/* { dg-do compile } */
/* { dg-additional-options "-march=armv8.2-a+dotprod" } */

int main ()
{
  return 0;
}

/* { dg-final { scan-assembler {\.arch armv8\.2-a\+dotprod\+crc\n} { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler {\.arch armv8\.2-a\+dotprod} { target *-*-darwin* } } } */

 /* dotprod needs to be emitted pre armv8.4.  */
