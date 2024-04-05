/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* darwinpcs D.10c classifies union members, skipping members of empty class
   type; a flexible array member has no size and must not crash that check.
   The union is not an HFA (it has an int member), so it is passed in a GPR,
   as clang does.  */

union U { float f; int a[]; };

/*
**f:
**	...
**	(?:str	w0|fmov	s0, w0).*
**	...
**	ret
*/
float
f (union U u)
{
  return u.f;
}

float ext (union U);

/*
**cu:
**	...
**	(?:ldr	x0|fmov	w0, s0).*
**	...
**	b	_?ext
*/
float
cu (float x)
{
  union U u = { x };
  return ext (u);
}
