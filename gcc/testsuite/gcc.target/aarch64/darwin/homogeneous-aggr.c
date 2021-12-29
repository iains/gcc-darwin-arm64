/* { dg-do compile } */
/* { dg-additional-options "-O " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

typedef struct sf { float a; float b; float c;} SF;

__attribute__((__noinline__)) void
hmg_f (SF a);

/* we should use registers for each item
**call_hmg_f:
**	...
**	fmov	s0, 1.0e\+0
**	fmov	s1, 2.0e\+0
**	fmov	s2, 3.0e\+0
**	bl	_hmg_f
**	...
*/

void
call_hmg_f (void)
{
 SF A = { 1.0F, 2.0F, 3.0F };
 hmg_f (A);
}
