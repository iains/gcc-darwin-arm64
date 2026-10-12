/* { dg-do link } */
/* { dg-skip-if "Apple silicon needs no Cortex errata workarounds, and ld64 has no such option" { *-*-darwin* } } */
/* { dg-additional-options "-march=armv8-a -mfix-cortex-a53-843419 -###" } */

int main()
{
  return 0;
}

/* { dg-message "-mfix-cortex-a53-843419" "note" { target *-*-* } 0 } */
/* { dg-message "--fix-cortex-a53-843419" "note" { target *-*-* } 0 } */
/* { dg-excess-errors "" } */
