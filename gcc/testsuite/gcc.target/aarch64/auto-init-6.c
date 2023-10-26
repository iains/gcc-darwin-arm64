/* Verify pattern initialization for complex type automatic variables.  */
/* { dg-do compile } */
/* { dg-options "-ftrivial-auto-var-init=pattern" } */
/* The unwind tables are not the subject of this test and it has no function
   body scans, so switch them off on Darwin.  Both options are needed.  Without
   them the CIE, which Darwin emits as data directives because its assembler has
   no .cfi_* support, would contribute a further ".long 0".  */
/* { dg-additional-options "-fno-unwind-tables -fno-asynchronous-unwind-tables" { target *-*-darwin* } } */


_Complex long double result;

_Complex long double foo()
{
  _Complex float temp1;
  _Complex double temp2;
  _Complex long double temp3;

  result = temp1 + temp2 + temp3;
  return result;
}

/* Mach-O's ".word" is a 16-bit entity, so Darwin spells a 4-byte datum ".long".  */
/* { dg-final { scan-assembler-times "word\t-16843010" 14  { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler-times "\.long\t-16843010" 14 { target *-*-darwin* } } } */
