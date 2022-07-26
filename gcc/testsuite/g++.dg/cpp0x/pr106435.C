// PR c++/106435
// { dg-do run { target c++11 } }
// { dg-additional-sources "pr106435-b.cc" }

#include "pr106435.h"

int num_calls = 0;

extern "C" __attribute__((__noreturn__)) void abort();

thread_local Foo Bar::bat;

int main() {
  int v = Bar::foo.func();
  if (v != 2)
    abort();
  v = Bar::bat.func();
  if (v != 3)
    abort();
}
