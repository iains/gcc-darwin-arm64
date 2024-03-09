// PR c++/109164
// { dg-do run { target c++11 } }
// { dg-options "-O2" }
// { dg-add-options tls }
// { dg-require-effective-target tls_runtime }
// { dg-additional-sources "thread_local13-aux.cc" }
// { dg-xfail-run-if "PR112294/106435" { *-*-darwin* } }

struct S { virtual void foo (); int s; };
extern thread_local S &t;
bool bar ();

bool
baz ()
{
  while (1)
    {
      t.foo ();
      if (!bar ())
        return false;
    }
}
