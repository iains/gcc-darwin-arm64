/* { dg-do compile } */
/* { dg-additional-options "-march=armv9.3-a+nopredres+nols64+nomops" } */

int main ()
{
  return 0;
}

/* { dg-final { scan-assembler-times {\.arch armv9\.3\-a\+crc\+nopredres\+nomops\n} 1 { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler-times {\.arch armv9\.3\-a\+nopredres\+nomops} 1 { target *-*-darwin* } } } */