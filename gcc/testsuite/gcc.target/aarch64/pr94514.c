/* PR target/94514. Unwind across mixed pac-ret and non-pac-ret frames.  */
/* { dg-do run } */
/* { dg-require-effective-target lp64 } */
/* The Darwin unwinder comes from libSystem and does not act on
   DW_CFA_AARCH64_negate_ra_state, so unwinding through a frame whose return
   address is signed fails.  Apple clang fails this identically, and both
   toolchains emit correct unwind data, so this is an OS limitation rather than
   a codegen bug.  An XPASS here means the OS unwinder has gained support.  */
/* { dg-xfail-run-if "OS unwinder ignores DW_CFA_AARCH64_negate_ra_state" { aarch64*-*-darwin* } } */
/* { dg-options "-fexceptions -O2" } */

#include <unwind.h>
#include <stdlib.h>
#include <stdio.h>

#define die() \
  do { \
    printf ("%s:%d: reached unexpectedly.\n", __FILE__, __LINE__); \
    fflush (stdout); \
    abort (); \
  } while (0)

static struct _Unwind_Exception exc;

static _Unwind_Reason_Code
force_unwind_stop (int version, _Unwind_Action actions,
                   _Unwind_Exception_Class exc_class,
                   struct _Unwind_Exception *exc_obj,
                   struct _Unwind_Context *context,
                   void *stop_parameter)
{
  printf ("%s: CFA: %p PC: %p actions: %d\n",
	  __func__,
	  (void *)_Unwind_GetCFA (context),
	  (void *)_Unwind_GetIP (context),
	  (int)actions);
  if (actions & _UA_END_OF_STACK)
    die ();
  return _URC_NO_REASON;
}

static void force_unwind (void)
{
#ifndef __USING_SJLJ_EXCEPTIONS__
  _Unwind_ForcedUnwind (&exc, force_unwind_stop, 0);
#else
  _Unwind_SjLj_ForcedUnwind (&exc, force_unwind_stop, 0);
#endif
}

__attribute__((noinline, target("branch-protection=pac-ret")))
static void f2_pac_ret (void)
{
  force_unwind ();
  die ();
}

__attribute__((noinline, target("branch-protection=none")))
static void f1_no_pac_ret (void)
{
  f2_pac_ret ();
  die ();
}

__attribute__((noinline, target("branch-protection=pac-ret")))
static void f0_pac_ret (void)
{
  f1_no_pac_ret ();
  die ();
}

static void cleanup_handler (void *p)
{
  printf ("%s: Success.\n", __func__);
  exit (0);
}

int main ()
{
  char dummy __attribute__((cleanup (cleanup_handler)));
  f0_pac_ret ();
  die ();
}
