// Shared "header" for declone6.C and declone6-aux.cc.  The definitions must be
// inline to live here, which is the point: the explicit instantiation decl
// below tells every includer not to emit them, and exactly one TU overrides it with
// an explicit instantiation def to become the designated emitter,
// declone6-aux.cc
//
// Shaped after std::basic_string, which triggered this case.

template <typename T>
struct B
{
  T n;

  constexpr B (T x) : n (x)
  {
    for (int i = 0; i < 16; i++)
      n += (x ^ i) + (n >> 1);
  }

  constexpr B (const B &o) : n (o.n) { n += 1; }

  constexpr ~B () { n = 0; }
};

// The declaration: "do not instantiate the definitions here."
extern template struct B<int>;
