/* { dg-lto-do run } */

#include "../nop.h"

asm (".globl start_\n\t.p2align 2\nstart_: " NOP);

int
main ()
{
  return 0;
}
