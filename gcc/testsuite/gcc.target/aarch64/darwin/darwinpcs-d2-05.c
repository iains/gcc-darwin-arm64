/* { dg-do compile } */
/* { dg-additional-options "-O" } */

/* A plain (non-homogeneous) struct passed on the stack after an earlier
   floating-point argument.  The classification of the struct must not
   depend on the earlier argument; it used to, and a checking compiler
   then failed an assert in aarch64_function_arg_boundary_ca.  */

struct s { int a, b; };

void f (double d, long x0, long x1, long x2, long x3, long x4, long x5,
	long x6, long x7, long x8, struct s v);

void
g (void)
{
  struct s v = { 1, 2 };
  f (1.0, 0, 1, 2, 3, 4, 5, 6, 7, 8, v);
}

void
h (double d, long x0, long x1, long x2, long x3, long x4, long x5,
   long x6, long x7, long x8, struct s v)
{
  f (d, x0, x1, x2, x3, x4, x5, x6, x7, x8, v);
}
