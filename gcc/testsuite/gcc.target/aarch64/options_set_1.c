/* { dg-do compile } */
/* { dg-additional-options "-march=armv8.2-a" } */

int main ()
{
  return 0;
}

/* Darwin's assembler does not need this fix, and we need to omit it.  */
/* { dg-final { scan-assembler-times {\.arch armv8\.2\-a\+crc} 1 { xfail *-*-darwin* } } } */

/* Check to see if crc is output by default.  */
