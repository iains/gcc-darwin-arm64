/* { dg-do "compile" } */
/* { dg-options "-O1 -fpatchable-function-entry=1 -fasynchronous-unwind-tables" } */

/* Test the placement of the .LPFE0 label.  */

void
foo (void)
{
}

/* PR98776 was that the patchable area was emitted BEFORE the start of the
   function's unwind range; both scans below assert that it is not.  Where the
   assembler supports CFI directives, that range begins at .cfi_startproc.
   Otherwise GCC emits FUNC_BEGIN_LABEL, which the hand-built FDE references as
   its initial location - the same information.  The .LPFE0 label belongs to the
   __patchable_function_entries table, which is not emitted where there is no
   consumer for it (e.g. Mach-O), and is not what this test is about.  */
/* { dg-final { scan-assembler "\t\.cfi_startproc\n.*\.LPFE0:\n\tnop\n\tret\n" { target cfi } } } */
/* { dg-final { scan-assembler "_?foo:\n\\.?LFB0:\n\tnop\n\tret\n" { target *-*-darwin* } } } */
