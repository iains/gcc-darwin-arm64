/* PR target/94514. Unwind across mixed pac-ret and non-pac-ret frames.  */
/* { dg-do run } */
/* { dg-require-effective-target lp64 } */
/* The Darwin unwinder comes from libSystem and does not act on
   DW_CFA_AARCH64_negate_ra_state, so unwinding through a frame whose return
   address is signed fails.  Apple clang fails this identically, and both
   toolchains emit correct unwind data, so this is an OS limitation rather than
   a codegen bug.  An XPASS here means the OS unwinder has gained support.  */
/* { dg-xfail-run-if "OS unwinder ignores DW_CFA_AARCH64_negate_ra_state" { aarch64*-*-darwin* } } */

__attribute__((noinline, target("branch-protection=pac-ret")))
static void do_throw (void)
{
  throw 42;
  __builtin_abort ();
}

__attribute__((noinline, target("branch-protection=none")))
static void no_pac_ret (void)
{
  do_throw ();
  __builtin_abort ();
}

int main ()
{
  try {
    no_pac_ret ();
  } catch (...) {
    return 0;
  }
  __builtin_abort ();
}
