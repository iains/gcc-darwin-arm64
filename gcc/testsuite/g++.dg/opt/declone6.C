// PR c++/111019
// The explicit instantiation declaration case, with no library involved.
//
// declone6.h holds inline definitions plus `extern template struct B<int>;', so
// the bodies are VISIBLE in both TUs while neither is told to emit them, and
// declone6-aux.cc is the one designated emitter.  The program is exactly what
// [temp.explicit] asks for.
//
// At -Os the clones become forwarding thunks to a unified C4 body.  Inlining a
// thunk here materialises a call to that C4 -- but no source can name a C4, so
// no TU can be asked to export one, and this TU is not the emitter.  Before the
// fix this TU was left referencing __ZN1BIiEC4Ei; after it, it references the
// C1 and C2 clones, which are exactly the symbols the emitter publishes.
//
// ⛔⛔ THE RUN IS NOT THE GUARD, AND THE DISTINCTION IS THE WHOLE POINT OF THE
// PR.  Measured unpatched: this program LINKS AND RUNS, because the emitter TU
// is compiled with the same -Os and so happens to publish a C4 of its own.  A
// C4 exists only where -fdeclone-ctor-dtor was used, so that is luck, not
// contract -- in the reported case the emitter was a prebuilt libstdc++ and the
// link failed.  ⇒ the scans below discriminate; the run does not.
//
// { dg-do run }
// { dg-options "-std=gnu++20 -Os -save-temps" }
// { dg-additional-sources "declone6-aux.cc" }
// { dg-final { scan-assembler-not "_ZN1BIiEC4Ei" { target { ! comdat_group } } } }
// { dg-final { scan-assembler "_ZN1BIiEC\[12\]Ei" { target { ! comdat_group } } } }

#include "declone6.h"

struct D : B<int>			// derived: uses B<int>::C2 (base-object ctor)
{
  D (int x) : B<int> (x) {}
};

B<int> *p;
D *q;

int
main ()
{
  p = new B<int> (2);			// uses B<int>::C1 (complete-object ctor)
  q = new D (5);
  return !(p->n != 0 && q->n != 0 && b_probe != 0);
}
