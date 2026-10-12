/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 1000
short a[N] = {0};
short b[N] = {0};

/* The Darwin body does not end at the return: the narrow return is promoted in
   the callee (the D.3 rule), and the resulting and+sxth pairs are duplicated
   into tails placed after it.  MEASURED, one variable changed: with an int
   return the tails disappear and the body ends at the return.  So this is the
   promotion family -- the same class as vec-init-23.c and pr125538.c -- and not
   a difference the ABI requires of the generated code.
   XFAIL rather than a Mach-O body, so the row XPASSes if that improves.  */
/*
** foo: { xfail *-*-darwin* }
**	...
**	cmgt	v[0-9]+\.8h, v[0-9]+\.8h, v[0-9]+\.8h
**	umaxp	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	...
**	ldrsh	w[0-9]+, \[[^\n]+\]
**	ldrsh	w[0-9]+, \[[^\n]+\]
**	cmp	w[0-9]+, w[0-9]+
**	...
**	ret
*/
__attribute__ ((noipa, noinline))
short foo (void)
{
  for (unsigned short i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return i;
    }
  return 0;
}
