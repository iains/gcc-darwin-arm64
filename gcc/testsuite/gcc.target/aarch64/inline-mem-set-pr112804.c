/* { dg-do compile } */
/* { dg-skip-if "-mabi=ilp32 unsupported" { aarch64-*-darwin* } } */
/* { dg-options "-finline-stringops -mabi=ilp32 -Wno-deprecated -ftrivial-auto-var-init=zero" } */

short m(unsigned k) {
  const unsigned short *n[65];
  return 0;
}
