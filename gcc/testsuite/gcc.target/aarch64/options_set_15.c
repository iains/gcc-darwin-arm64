/* { dg-do compile } */
/* { dg-additional-options "-march=armv8.2-a+fp16fml+fp" } */

int main ()
{
  return 0;
}

/* { dg-final { scan-assembler {\.arch armv8\.2-a\+crc\+fp16fml*\n} { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler {\.arch armv8\.2-a\+fp16fml*} { target *-*-darwin* } } } */

 /* fp included in fp16fml, only emit latter.  */
