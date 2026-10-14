/* { dg-do "compile" } */
/* { dg-options "-O1 -fpatchable-function-entry=1 -fasynchronous-unwind-tables" } */

/* Test the placement of the .LPFE0 label.  */

void
foo (void)
{
}

/* The .cfi_* directives are only emitted where the assembler supports them.  */
/* { dg-final { scan-assembler "\t\.cfi_startproc\n.*\.LPFE0:\n\tnop\n\tret\n" { target cfi } } } */
