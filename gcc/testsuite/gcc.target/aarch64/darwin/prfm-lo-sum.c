/* { dg-do assemble } */
/* { dg-options "-O1 -save-temps" } */

/* ld64 rejects a PAGEOFF12 relocation on PRFM, so a prefetch of a global
   must not fold the low part of the symbol address into the PRFM.  When
   several prefetches share one ADRP, the later ones would otherwise use a
   LO_SUM address directly.  */

struct S { short a; int b; long d; } gs;

void
pg (void)
{
  __builtin_prefetch (&gs, 0, 0);
  __builtin_prefetch (&gs.b, 0, 0);
  __builtin_prefetch (&gs.d, 0, 0);
}

/* { dg-final { scan-assembler-times {prf[u]?m\t} 3 } } */
/* { dg-final { scan-assembler-not {prf[u]?m\t[^\n]*@PAGEOFF} } } */
