/* { dg-do assemble } */
/* { dg-options "-O2 -save-temps" } */

/* The Darwin assembler does not substitute PRFUM for a PRFM whose offset is
   negative or not a multiple of 8, so those must be emitted as PRFUM, and
   offsets valid for neither form must be legitimized first.  */

void pm8 (const char *p) { __builtin_prefetch (p - 8, 0, 3); }
void pm256 (const char *p) { __builtin_prefetch (p - 256, 0, 3); }
void pm264 (const char *p) { __builtin_prefetch (p - 264, 0, 3); }
void pp3 (const char *p) { __builtin_prefetch (p + 3, 0, 3); }
void pp255 (const char *p) { __builtin_prefetch (p + 255, 0, 3); }
void pp8 (const char *p) { __builtin_prefetch (p + 8, 0, 3); }
void pp257 (const char *p) { __builtin_prefetch (p + 257, 0, 3); }
void pp32760 (const char *p) { __builtin_prefetch (p + 32760, 0, 3); }
void pp32768 (const char *p) { __builtin_prefetch (p + 32768, 0, 3); }
void pw3 (char *p) { __builtin_prefetch (p + 3, 1, 3); }
void pi3 (const char *p) { __builtin_prefetch (p + 3, 2, 3); }

/* { dg-final { scan-assembler-times {prfum\tPLDL1KEEP, \[x0, -8\]} 1 } } */
/* { dg-final { scan-assembler-times {prfum\tPLDL1KEEP, \[x0, -256\]} 1 } } */
/* { dg-final { scan-assembler-times {prfum\tPLDL1KEEP, \[x0, 3\]} 2 } } */
/* { dg-final { scan-assembler-times {prfum\tPLDL1KEEP, \[x0, 255\]} 1 } } */
/* { dg-final { scan-assembler-times {prfum\tPSTL1KEEP, \[x0, 3\]} 1 } } */
/* { dg-final { scan-assembler-times {prfum\t} 6 } } */
/* { dg-final { scan-assembler-times {prfm\tPLDL1KEEP, \[x0, 8\]} 1 } } */
/* { dg-final { scan-assembler-times {prfm\tPLDL1KEEP, \[x0, 32760\]} 1 } } */
/* { dg-final { scan-assembler-times {prfm\tPLDL1KEEP, \[x0\]} 3 } } */
/* { dg-final { scan-assembler-not {prfm\t[A-Z0-9]+, \[x[0-9]+, -} } } */
