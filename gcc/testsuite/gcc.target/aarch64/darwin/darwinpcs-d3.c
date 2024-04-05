/* { dg-do compile } */
/* { dg-additional-options "-O " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* darwinpcs D.3: the caller sign- or zero-extends integer arguments narrower
   than 32 bits, so the callee uses them without extending them again.

**foo:
**	cmp	w0, w1
**	cset	w0, eq
**	ret
*/

__attribute__((__noinline__))
int
foo (char a, unsigned char b)
{
  return a == b ? 1 : 0;
}

/*
**bar:
**	cmp	w0, w1
**	cset	w0, eq
**	ret
*/

__attribute__((__noinline__))
int
bar (short a, unsigned short b)
{
  return a == b ? 1 : 0;
}

/* The caller side: each narrow argument is extended to 32 bits before the
   call.

**call_foo:
**	...
**	and	w1, w1, 255
**	sxtb	w0, w0
**	bl	_foo
**	...
*/

int
call_foo (int a, int b)
{
  return foo (a, b);
}

/*
**call_bar:
**	...
**	and	w1, w1, 65535
**	sxth	w0, w0
**	bl	_bar
**	...
*/

int
call_bar (int a, int b)
{
  return bar (a, b);
}
