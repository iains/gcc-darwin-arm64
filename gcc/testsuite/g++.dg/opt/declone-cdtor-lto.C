// PR c++/125359

// Check that we do not leave a call to a unified cdtor in an LTO build.
// { dg-do run { target c++14 } }
// { dg-require-effective-target lto }
// { dg-options "-Os -flto -fstrict-aliasing" }
// { dg-skip-if "requires hosted libstdc++ for cassert" { ! hostedlib } }

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
