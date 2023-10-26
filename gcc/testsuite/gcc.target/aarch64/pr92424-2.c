/* { dg-do "compile" } */
/* { dg-options "-O1" } */

/* Test the placement of the .LPFE0 label.  */

void
__attribute__ ((target("branch-protection=bti"),
		patchable_function_entry (1, 0)))
f10_bti ()
{
}
/* Mach-O emits no __patchable_function_entries table and no LPFE label (as
   clang does), so there is no label to locate the patch area with; the
   ordering this PR is about - the NOP placed after the BTI - is asserted
   directly instead.  */
/* { dg-final { scan-assembler "hint\t34 // bti c\n.*\.LPFE0:\n\tnop\n.*\tret\n" { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler "hint\t34 // bti c\n\tnop\n.*\tret\n" { target *-*-darwin* } } } */
