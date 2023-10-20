/* { dg-do compile } */
/* { dg-additional-options "-march=armv8.2-a" } */

int main ()
{
  return 0;
}

/* { dg-final { scan-assembler-times {\.arch armv8\.2\-a\+crc\n} 1 { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler-times {\.arch armv8\.2\-a\n} 1 { target *-*-darwin* } } } */

/* Check to see if crc is output by default (not on Darwin).  */
