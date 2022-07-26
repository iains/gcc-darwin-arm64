// PR c++/106435
#pragma once

extern int num_calls;
struct Foo {
  Foo();
  Foo(int _x) : x(_x) {}
  int func();
  int getX () { return x; }
  int x;
};

struct Bar {
  thread_local static Foo foo;
  thread_local static Foo baz;
  thread_local static Foo bat;
};

extern thread_local void* tl;

inline void* gt_tl () {return tl;}

extern thread_local Foo F;

extern thread_local void* tl1;

inline void* gt_tl1 () {return tl1;}
