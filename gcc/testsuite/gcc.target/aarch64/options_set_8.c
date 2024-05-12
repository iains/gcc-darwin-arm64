/* { dg-do compile } */
/* { dg-additional-options "-march=armv8.4-a+nodotprod" } */

int main ()
{
  return 0;
}

/* { dg-final { scan-assembler-times {\.arch armv8\.4\-a\+crc\+nodotprod\n} 1 { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler-times {\.arch armv8\.4\-a} 1 { target *-*-darwin* } } } */
/* { dg-final { scan-assembler-not {\+dotprod} } } */

/* Checking if trying to turn off default features propagates the commandline
   option.  */
