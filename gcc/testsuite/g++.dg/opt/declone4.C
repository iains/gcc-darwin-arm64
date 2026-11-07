// PR c++/111019
// A cdtor whose linkage is DECIDED and which this translation unit therefore
// emits must still be decloned: the C1/C2 clones become thunks and the unified
// C4 body is emitted.  That body must not be published.  Whether a C4 exists at
// all is a private consequence of the flags the defining TU was built with, so
// no other TU may name one.
//
// B has a virtual base so that can_alias_cdtor fails and the thunking path is
// taken on every target rather than only on those without aliases.  B::B is an
// ordinary out-of-line member, so the cdtor has known, non-vague linkage and
// maybe_thunk_body can claim the unified body for this TU outright (it is made
// TU-local).  The vague-linkage case, where it cannot, is declone5.C.
//
// This test is an INVARIANT guard, not coverage of the defect: it passes both
// with and without the fix, and exists so that declining to declone does not
// grow to cover cdtors whose unified body this TU does emit.
// { dg-do run }
// { dg-options "-fdeclone-ctor-dtor -save-temps" }
// { dg-final { scan-assembler "_ZN1BC4Ei" } }
// { dg-final { scan-assembler-not "globl\[^\n\]*_ZN1BC4Ei" } }

struct V { int v; V (); };
V::V () : v (1) {}

struct B : virtual V
{
  int n;
  B (int);
};

B::B (int x) : n (x * 3 + 1) {}

struct D : B				// uses B::C2
{
  D (int x) : B (x) {}
};

B *p;
D *q;

int
main ()
{
  p = new B (2);			// uses B::C1
  q = new D (5);
  return !(p->n == 7 && q->n == 16 && p->v == 1 && q->v == 1);
}
