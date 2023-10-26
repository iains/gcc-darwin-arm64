/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-skip-if "SVE is not available on Darwin" { *-*-darwin* } } */
/* { dg-options "-fbuilding-libgcc" } */

#pragma GCC target "+sve"

#include "dwarf_reg_size_1.c"
