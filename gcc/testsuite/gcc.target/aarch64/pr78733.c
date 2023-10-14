/* { dg-do compile } */
/* { dg-options "-O2 -mcmodel=large -mpc-relative-literal-loads -fno-pie" } */
/* { dg-require-effective-target lp64 } */
/* { dg-skip-if "-mcmodel=large, no support for -fpic" { aarch64-*-* }  { "-fpic" } { "" } } */
/* { dg-xfail-if "issue #100" { *-*-darwin* } } */

__int128
t (void)
{
  return ((__int128)0x123456789abcdef << 64) | 0xfedcba987654321;
}

/* { dg-final { scan-assembler "adr" { xfail { *-*-darwin* } } } } */
/* { dg-final { scan-assembler-not "adrp" } } */
