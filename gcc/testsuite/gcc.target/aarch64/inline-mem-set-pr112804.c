/* { dg-do compile } */
/* { dg-options "-finline-stringops -mabi=ilp32 -ftrivial-auto-var-init=zero" } */
/* { dg-skip-if "-mabi=ilp32 unsupported" { aarch64-*-darwin* } } */

short m(unsigned k) {
  const unsigned short *n[65];
  return 0;
}
