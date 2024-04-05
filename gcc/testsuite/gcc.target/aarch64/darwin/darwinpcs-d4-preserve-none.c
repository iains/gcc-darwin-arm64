/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* darwinpcs D.4 with preserve_none: the argument registers are x20-x28,
   x0-x7, x10-x14, x9, x15, so a 16-byte argument can start at a register
   whose successor in that order is not the next register.  It must be
   passed in the two registers the order gives (as clang does), not in an
   even/odd pair.  */

typedef __int128 T;
#define PN __attribute__((preserve_none))

/* x28 then x0.
**p8:
**	mov	x1, x0
**	mov	x0, x28
**	ret
*/
PN T
p8 (long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7,
    T t)
{
  return t;
}

/* x7 then x10.
**p16:
**	mov	x0, x7
**	mov	x1, x10
**	ret
*/
PN T
p16 (long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7,
     long b0, long b1, long b2, long b3, long b4, long b5, long b6, long b7,
     T t)
{
  return t;
}

/* x21 then x22.
**p1:
**	mov	x0, x21
**	mov	x1, x22
**	ret
*/
PN T
p1 (long a0, T t)
{
  return t;
}
