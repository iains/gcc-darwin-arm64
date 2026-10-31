// { dg-do compile { target c++98_only } }
// { dg-options "-Wc++11-compat" }

// Darwin switches -fallow-extra-keywords on by default (clang compatibility,
// for SDK headers), which makes _Alignas and _Alignof keywords in C++ there;
// turn it off so that the identifiers below mean what this test intends.
// { dg-additional-options "-fno-allow-extra-keywords" { target *-*-darwin* } }

// Validate suppression of -Wc++11-compat diagnostics.
#pragma GCC diagnostic ignored "-Wc++11-compat"
int alignof;
int alignas;
int constexpr;
int decltype;
int noexcept;
int nullptr;
int static_assert;
int thread_local;
int _Alignas;
int _Alignof;
int _Thread_local;
