/* { dg-do link } */
/* { dg-skip-if "Apple silicon needs no Cortex errata workarounds, and ld64 has no such option" { *-*-darwin* } } */
/* { dg-additional-options "-mfix-cortex-a53-835769 -march=armv9-a -###" } */

int main()
{
  return 0;
}

/* { dg-message "-mno-fix-cortex-a53-835769" "note" { target *-*-* } 0 } */
/* { dg-excess-errors "" } */
