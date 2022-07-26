// PR c++/106435
#pragma once

extern int num_calls;
struct Foo {
  Foo();
  int func();
};

struct Bar {
  thread_local static Foo foo;
  thread_local static Foo baz;
  thread_local static Foo bat;
};
