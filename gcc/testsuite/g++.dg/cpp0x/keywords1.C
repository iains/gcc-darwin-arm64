// PR c++/106111
// { dg-do compile { target c++98_only } }
// { dg-options "-Wc++11-compat" }

// Darwin switches -fallow-extra-keywords on by default (clang compatibility,
// for SDK headers), which makes _Alignas and _Alignof keywords in C++ there;
// turn it off so that the identifiers below mean what this test intends.
// { dg-additional-options "-fno-allow-extra-keywords" { target *-*-darwin* } }

int alignof; // { dg-warning "is a keyword in C\\\+\\\+11" }
int alignas; // { dg-warning "is a keyword in C\\\+\\\+11" }
int constexpr; // { dg-warning "is a keyword in C\\\+\\\+11" }
int decltype; // { dg-warning "is a keyword in C\\\+\\\+11" }
int noexcept; // { dg-warning "is a keyword in C\\\+\\\+11" }
int nullptr; // { dg-warning "is a keyword in C\\\+\\\+11" }
int static_assert; // { dg-warning "is a keyword in C\\\+\\\+11" }
int thread_local; // { dg-warning "is a keyword in C\\\+\\\+11" }
int _Alignas;
int _Alignof;
int _Thread_local;
