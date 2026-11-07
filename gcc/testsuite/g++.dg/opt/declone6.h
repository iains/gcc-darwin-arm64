// Shared "header" for declone6.C and declone6-aux.cc.  The definitions must be
// inline to live here, which is the point: the explicit instantiation
// DECLARATION below tells every includer not to emit them, and exactly one TU
// overrides it with an explicit instantiation DEFINITION to become the
// designated emitter (declone6-aux.cc).
//
// Shaped after std::basic_string, which is the real casualty, in three respects
// that are all load-bearing:
//
//   NO virtual base.  clone_cdtor clones cdtors unconditionally, so C1/C2/C4
//     exist regardless; basic_string has no virtual base either.
//   constexpr cdtors, as basic_string's are from C++20 via _GLIBCXX20_CONSTEXPR.
//   ⭐ The declaration comes AFTER a constant-expression use, not immediately
//     after the class.  bits/basic_string.tcc carries its `extern template class
//     basic_string<char>' at the END of the header, after the inline -- and from
//     C++20 constexpr -- bodies.  A constexpr cdtor is instantiated at its first
//     use in a constant expression, so it can be instantiated, and cloned,
//     before the declaration that says not to emit it has been seen.
//
// Measured 2x2 on this shape, unpatched, -std=gnu++20 -Os: only
// declaration-after-a-constexpr-use leaves an undefined C4 behind.
//
//     declaration before the use, constexpr    0 undefined C4
//     declaration before the use, plain        0
//     declaration AFTER the use, constexpr     1   <-- this file
//     declaration AFTER the use, plain         0
//
// ⛔ So moving the declaration up, or dropping constexpr, silently turns this
// file into a test that passes either way.

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

// The constant-expression use, before the declaration below.
constexpr int b_probe = B<int> (1).n;

// Only now: "do not instantiate the definitions here."
extern template struct B<int>;
