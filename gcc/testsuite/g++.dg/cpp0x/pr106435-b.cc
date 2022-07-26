// PR c++/106435
#include "pr106435.h"

//#include <iostream>

Foo::Foo() {
  ++num_calls;
//  std::cout << "Foo::Foo(this=" << this << ")\n";
}

int Foo::func() {
//  std::cout << "Foo::func(this=" << this << ")\n";
  return num_calls;
}

thread_local Foo Bar::foo;
thread_local Foo Bar::baz;
