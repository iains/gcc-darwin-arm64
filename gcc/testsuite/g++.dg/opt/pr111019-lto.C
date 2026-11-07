// PR c++/111019
// Pin the -Os -flto combination, which the torture suite does not reach.
//
// g++.dg/torture/pr111019.C covers this reproducer across the torture option
// sets, but LTO_TORTURE_OPTIONS is made of -O2 -flto variants only, and the
// bug needs -Os: that is what turns on -fdeclone-ctor-dtor, which splits a
// cdtor into a unified body (C4) plus C1/C2 thunks.  Inlining a thunk then
// materialises a call to the C4, and a C4 exists only where the DEFINING
// translation unit was built with -fdeclone-ctor-dtor.  libstdc++ is not, so
// the reference is undefined and the link fails.  The -O2 -flto arms pass
// either way, so without this test the -flto half of the option space is
// unguarded.
//
// The requirement on libstdc++ is the point rather than an accident: the
// missing definition has to come from a TU outside our control that was
// compiled without the flag, and the library supplies exactly that.
//
// The standard is pinned, and it has to be: measured at -Os, the undefined C4
// appears at -std=gnu++20 and at no other standard from gnu++11 to gnu++29, for
// this shape and for g++.dg/torture/pr111019.C.  libstdc++ gates
// `extern template class basic_string<char>' on __cplusplus <= 202002L
// (bits/basic_string.tcc), so from C++23 this TU instantiates std::string
// itself and defines the C4 it calls; what excludes C++11 through C++17 is not
// established.  Without the pin the test would cover the defect only while the
// DEFAULT standard happens to be gnu++20, which is the trap
// g++.dg/torture/pr111019.C is already in.
// { dg-do run }
// { dg-require-effective-target lto }
// { dg-options "-std=gnu++20 -Os -flto -fstrict-aliasing" }
// { dg-skip-if "requires hosted libstdc++ for cassert" { ! hostedlib } }
// The run is a LINK result: it would also pass against a libstdc++ that had
// itself been built at -Os, because such a library exports a C4 of its own.
// Reading the executable's symbol table with gcc-nm is what separates the two
// -- an import that no source can name survives there even when the link
// succeeds.
// Unpatched, this does not link at all, so the link is the guard here; the
// symbol scan asserts the positive outcome.  A bare pattern, for the reason
// given in pr111019-2.C.
// { dg-final { scan-symbol {C1EO} } }

#include <cassert>
#include <memory>
#include <string>

class Base
{
public:
  Base* previous = nullptr;
  Base* next = nullptr;
  Base* target = nullptr;
};

class Target : public Base
{
public:
  __attribute__((always_inline)) ~Target()
  {
    while (this->next)
    {
      Base* n = this->next;

      if (n->previous)
        n->previous->next = n->next;
      if (n->next)
        n->next->previous = n->previous;
      n->previous = nullptr;
      n->next = nullptr;
      n->target = nullptr;
    }
  }
};

template <typename T>
class TargetWithData final : public Target
{
public:
  TargetWithData(T data)
    : data(data)
  {}
  T data;
};

void test()
{
  Base ptr;
  {
    auto data = std::make_unique<TargetWithData<std::string>>(std::string("asdf"));
    ptr.target = &*data;
    ptr.previous = &*data;
    data->next = &ptr;

    assert(ptr.target != nullptr);
  }
  assert(ptr.target == nullptr);
}

int main(int, char**)
{
  test();
  return 0;
}
