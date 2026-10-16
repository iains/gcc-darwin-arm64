/* { dg-do compile } */
/* { dg-options "-O2 -fstack-clash-protection" } */

/* Check the DEFAULT stack-clash guard size, so no --param is given here.
   AArch64 defaults to 64KB, but a Darwin thread's default guard region is a
   single 16KB page (pthread_attr_getguardsize reports 16384), so a 64KB
   probing interval could step over it; Darwin therefore defaults to 4KB.

   A 5000-byte frame discriminates the two without depending on the exact
   probing sequence: it is far below a 64KB guard, so a 64KB default emits no
   probe at all, while a 4KB default must probe once before allocating the
   rest.  The sequences themselves are covered by stack-check-prologue-*.c.

   Only the Darwin side is asserted: this directory is driven by
   aarch64-darwin.exp, and the generic default can in any case be changed at
   configure time with --with-stack-clash-protection-guard-size.  */

void g (char *);

void
f (void)
{
  char buf[5000];
  g (buf);
}

/* The default has to survive the per-function path too, where the
   optimization table is re-applied for the new optimization level.  A target
   default set from SUBTARGET_OVERRIDE_OPTIONS rather than from the table
   would be reverted to the generic 16 for this function.  */

__attribute__((optimize("O1")))
void
f_per_function (void)
{
  char buf[5000];
  g (buf);
}

/* { dg-final { scan-assembler-times {sub\tsp, sp, #4096} 2 } } */
/* { dg-final { scan-assembler-times {str\txzr, \[sp, 1024\]} 2 } } */
