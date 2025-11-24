// { dg-do compile }
/* { dg-require-effective-target aarch64_asm_cmpbr_ok } */
// { dg-options "-O2" }

#pragma GCC target "+cmpbr"

typedef unsigned short Quantum;

double MagickMax(double x, double y) { return x > y ? x : y; }

void ConvertRGBToHCL(Quantum red, Quantum green) {
    if (red == MagickMax(red, green)) __builtin_abort();
}
