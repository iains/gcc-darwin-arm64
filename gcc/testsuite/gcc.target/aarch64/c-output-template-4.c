/* { dg-do compile } */
/* { dg-options "-O0" } */

void
test (void)
{
    __asm__ ("@ %c0" : : "S" (&test + 4));
}

/* { dg-final { scan-assembler "@ _?test\\+4" } } */
