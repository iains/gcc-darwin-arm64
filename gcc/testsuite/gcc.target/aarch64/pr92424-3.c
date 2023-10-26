/* { dg-do "compile" } */
/* { dg-options "-O1 -march=armv8-a" } */

/* Test the placement of the .LPFE0 label.  */

void
__attribute__ ((target("branch-protection=bti+pac-ret+leaf"),
		patchable_function_entry (1, 0)))
f10_pac ()
{
}
/* As for pr92424-2.c: Mach-O has no LPFE label to locate the patch area, so
   the ordering this PR is about - the NOP placed after the BTI - is asserted
   without one.  Everything else is identical: -march is pinned above, so the
   return is autiasp + ret on every target rather than depending on the
   configured default (at armv8.3-a or later it would be a single retaa).  */
/* { dg-final { scan-assembler "hint\t34 // bti c\n.*\.LPFE0:\n\tnop\n.*\thint\t25 // paciasp\n.*\thint\t29 // autiasp\n.*\tret\n" { target { ! *-*-darwin* } } } } */
/* { dg-final { scan-assembler "hint\t34 // bti c\n\tnop\n.*\thint\t25 // paciasp\n.*\thint\t29 // autiasp\n.*\tret\n" { target *-*-darwin* } } } */
