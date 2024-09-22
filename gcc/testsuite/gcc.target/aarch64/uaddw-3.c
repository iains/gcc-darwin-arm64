/* { dg-do compile } */
/* { dg-options "-O3" } */
/* { dg-additional-options "-fno-signed-char" { target *-*-darwin* } } */

#pragma GCC target "+nosve"

int 
t6(int len, void * dummy, unsigned char * __restrict x)
{
  len = len & ~31;
  unsigned short result = 0;
  __asm volatile ("");
  for (int i = 0; i < len; i++)
    result += x[i];
  return result;
}

/* { dg-final { scan-assembler "uaddw" } } */
/* { dg-final { scan-assembler "uaddw2" } } */
