// PR c++/111019
// The vague-linkage half of the pair.  A thunk and the unified body it calls
// stand in together for one cdtor, so they have to be selected together at
// link time.  For a vague-linkage cdtor that is what a COMDAT group provides.
// A target without groups has nothing to bind them with, so maybe_thunk_body
// can neither make the unified body TU-local (it is one-only) nor put it in a
// group -- and before this fix it was simply left public.
//
// A unified body is an implementation detail of the link unit rather than part
// of the interface: it exists only where -fdeclone-ctor-dtor was used.  So it
// must be emitted with hidden visibility, which still coalesces duplicate
// definitions within a DSO while keeping the symbol out of the export table.
// Nothing outside can then come to depend on a symbol whose existence depends
// on how this unit happened to be compiled.
//
// B has a virtual base so that can_alias_cdtor fails and the thunking path is
// taken; B<int> is an implicit instantiation, so its cdtor has vague linkage.
// The cdtors are noinline so that the symbols survive the -O levels a tester's
// board may add -- at -O3 the unified DESTRUCTOR body is still replaced by
// local isra/constprop clones, which is why only the constructor is scanned.
//
// On a target that does have COMDAT groups the group claims the body and this
// does not apply, hence the comdat_group gating.  declone4.C covers the case
// where the linkage is known and the body can be claimed outright.
// { dg-do run }
// { dg-options "-fdeclone-ctor-dtor -save-temps" }
// { dg-require-visibility "" }
// { dg-final { scan-assembler "_ZN1BIiEC4Ei" } }
// ⛔ scan-hidden, not a symbol-table scan: plain nm cannot see this.  Mach-O
// spells hidden visibility .private_extern, which keeps N_EXT set until the
// static link, so gcc-nm prints such a C4 as T exactly like a published one
// (only `nm -m' distinguishes it, as "weak private external").  scan-hidden
// already knows each target's spelling.
// { dg-final { scan-hidden "_ZN1BIiEC4Ei" { target { ! comdat_group } } } }
// The C1/C2 clones are the interface and must stay as visible as the cdtor was.
// { dg-final { scan-assembler "_ZN1BIiEC1Ei" } }
// { dg-final { scan-not-hidden "_ZN1BIiEC1Ei" } }

struct V { int v; V () : v (1) {} };

template <typename T>
struct B : virtual V
{
  T n;
  __attribute__((noinline)) B (T x) : n (x * 3 + 1) {}
  __attribute__((noinline)) ~B () { n = 0; }
};

struct D : B<int>			// uses B<int>::C2
{
  D (int x) : B<int> (x) {}
};

B<int> *p;
D *q;

int
main ()
{
  p = new B<int> (2);			// uses B<int>::C1
  q = new D (5);
  int r = !(p->n == 7 && q->n == 16 && p->v == 1 && q->v == 1);
  delete p;
  delete q;
  return r;
}
