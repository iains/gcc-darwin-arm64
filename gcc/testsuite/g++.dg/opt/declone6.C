// PR c++/125359

// declone6.h holds inline definitions plus `extern template struct B<int>;', so the
// bodies are VISIBLE in both TUs while neither is told to emit them.
// declone6-aux.cc overrides that with an explicit instantiation DEFINITION, which
// [temp.explicit] permits after the declaration, making it the one designated
// emitter.  The program is therefore exactly what the standard asks for.
//
// At -Os the clones become forwarding thunks to a unified C4 body; if a thunk is
// inlined here, the call to that C4 must not be left behind, because nothing in the
// source can name a C4 and so no TU can be asked to export one.
//
// { dg-do run { target c++20 } }
// { dg-options "-Os" }
// { dg-additional-sources "declone6-aux.cc" }

#include "declone6.h"

struct D : B<int>			// derived: uses B<int>::C2 (base-object ctor)
{
  D (int x) : B<int> (x) {}
};

struct Sink
{
  __attribute__((always_inline)) ~Sink () { total += v; }
  static int total;
  int v;
};
int Sink::total;

template <typename T>
struct Holder : Sink
{
  T held;
  Holder (const T &x) : held (x) { v = held.n; }
};

B<int> *p;
D *q;

__attribute__((noinline)) static int
use (int x)
{
  p = new B<int> (x);			// uses B<int>::C1 (complete-object ctor)
  q = new D (x);
  {
    B<int> t (x);
    Holder<B<int> > h (t);		// needs B<int>'s copy cdtor HERE, by value
    (void) h;
  }
  return p->n + q->n;
}

int
main ()
{
  int a = use (2);
  int b = use (2);
  return !(a == b && Sink::total != 0);
}
