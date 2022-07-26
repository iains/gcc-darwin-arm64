// PR c++/106435
// { dg-do run { target c++11 } }
// { dg-additional-sources "pr106435-b.cc" }

#include "pr106435.h"

int num_calls = 0;

thread_local Foo Bar::bat;

int main() {
  int v = Bar::foo.func();
  if (v != 2)
    __builtin_abort ();
  v = Bar::bat.func();
  if (v != 3)
    __builtin_abort ();
  if (F.getX() != 5)
    __builtin_abort();
  if  (gt_tl () != (void*)&F)
    __builtin_abort();
  if  (gt_tl1 ())
    __builtin_abort();
}
