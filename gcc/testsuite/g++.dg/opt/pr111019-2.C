// PR c++/111019
// The reporter's shape, reduced to three lines: one standard header and a move
// construction.  g++.dg/torture/pr111019.C covers the same defect but needs
// <memory>, <cassert>, an always_inline destructor chain and a user template.
//
// -Os turns on -fdeclone-ctor-dtor, which splits a cdtor into a unified body
// (C4) plus C1/C2 thunks.  Inlining a thunk materialises a call to the C4, and
// a C4 exists only where the DEFINING translation unit was built with
// -fdeclone-ctor-dtor.  libstdc++ is not, so before the fix the reference was
// left undefined here and the link failed.
//
// The standard is pinned, and that is not incidental: measured at -Os, the
// undefined C4 appears at -std=gnu++20 and at no other standard from gnu++11 to
// gnu++29.  Both bounds come from the one `extern template class
// basic_string<char>' declaration at the end of bits/basic_string.tcc:
//
//   above C++20 it is suppressed -- it is gated on __cplusplus <= 202002L so
//     that C++23 code can implicitly instantiate the members C++23 added -- so
//     this TU becomes the emitter and defines the C4 it calls;
//   below C++20 basic_string's cdtors are not constexpr (_GLIBCXX20_CONSTEXPR),
//     and a constexpr cdtor is what can be instantiated, and cloned, at a use
//     in a constant expression that PRECEDES the trailing declaration.  That
//     ordering is measured on g++.dg/opt/declone6.C, which reproduces the
//     defect with no library at all; for basic_string itself it is consistent
//     with both bounds but not independently confirmed.
//
// Pinning the standard keeps this test testing the defect when the default
// moves on.  ⚠ It does not keep the dependency on that declaration alive: if
// libstdc++ stops suppressing implicit instantiation here, this TU becomes the
// emitter and a "not" directive goes quiet.  The positive scan-symbol is the
// canary -- if it alone fails, this test needs a new shape and the fix has NOT
// regressed.
//
// The checks are not worth the same:
//
//   scan-assembler-not asks whether THIS TU emits a reference to a symbol no
//     source can name.  Provider-independent, but over-strong by construction:
//     it forbids the string outright where what is wrong is an UNDEFINED C4.
//   scan-symbol-not reads the final image's symbol table with gcc-nm, so it
//     separates a reference from a definition -- and it closes a hole the link
//     cannot.  Were the libstdc++ linked against itself built at -Os it would
//     export a C4, the link would succeed for the wrong reason, and the
//     undefined reference would survive in the executable as an import.
//   scan-symbol asserts the fix's positive outcome: the reference moved to the
//     C1 clone, a symbol the library does export.
//   the link and run reproduce the symptom as reported.
//
// A TU on a COMDAT-group target may legitimately reference a C4 it defines in
// the group, so the directives that assume otherwise are gated.  scan-symbol-not
// is not gated: an undefined C4 is wrong on every target.
// { dg-do run }
// { dg-options "-std=gnu++20 -Os -save-temps" }
// { dg-skip-if "requires hosted libstdc++ for <string>" { ! hostedlib } }
// { dg-final { scan-assembler-not "C4E" { target { ! comdat_group } } } }
// { dg-final { scan-assembler-not "D4E" { target { ! comdat_group } } } }
// The symbol scan reads the final image's symbol table with gcc-nm, which the
// assembler scans above cannot do: were the libstdc++ linked against itself
// built at -Os it would export a C4, the link would succeed for the wrong
// reason, and the bad reference would survive in the executable as an import.
//
// The pattern is deliberately a bare `C1EO' rather than an anchored
// "undefined symbol" regexp.  scan-symbol passes its argument list to
// scan-symbol-common as one word, so any pattern Tcl must quote -- anything
// containing whitespace, a bracket or a backslash -- arrives wrapped in literal
// braces and can never match.  A bare word needs no quoting and works as the
// directive is written today.
//
// What it asserts is therefore weaker but still discriminating: the reference
// moved to the C1 clone, a symbol the library exports.  Measured on
// x86_64-apple-darwin24, absent before the fix and present after, both plainly
// and under -flto.  It needs no target selector: where a translation unit
// legitimately emits its own cdtors -- a COMDAT-group target, or C++23 where the
// explicit instantiation declaration is suppressed -- C1EO is present as a
// definition instead, so the directive holds there too.
// { dg-final { scan-symbol {C1EO} } }

#include <string>

std::string *p;

int
main ()
{
  std::string x ("hello");
  p = new std::string (static_cast<std::string &&> (x));
  return *p != "hello";
}
