/* { dg-do assemble } */
/* { dg-options "-O2 -save-temps" } */

/* As prfm-prfum-offsets.c, for the ACLE __pldx and __pli intrinsics: the
   Darwin assembler does not substitute PRFUM for a PRFM whose offset is
   negative or not a multiple of 8.  */

#include <arm_acle.h>

void xm8 (const char *p) { __pldx (0, 0, 0, p - 8); }
void xp3 (const char *p) { __pldx (1, 0, 1, p + 3); }
void xp8 (const char *p) { __pldx (0, 1, 0, p + 8); }
void xp257 (const char *p) { __pldx (0, 0, 0, p + 257); }
void im16 (const char *p) { __pli (p - 16); }

/* { dg-final { scan-assembler-times {prfum\tPLDL1KEEP, \[x0, -8\]} 1 } } */
/* { dg-final { scan-assembler-times {prfum\tPSTL1STRM, \[x0, 3\]} 1 } } */
/* { dg-final { scan-assembler-times {prfum\tPLIL1KEEP, \[x0, -16\]} 1 } } */
/* { dg-final { scan-assembler-times {prfum\t} 3 } } */
/* { dg-final { scan-assembler-times {prfm\tPLDL2KEEP, \[x0, 8\]} 1 } } */
/* { dg-final { scan-assembler-times {prfm\tPLDL1KEEP, \[x0\]} 1 } } */
/* { dg-final { scan-assembler-not {prfm\t[A-Z0-9]+, \[x[0-9]+, -} } } */
