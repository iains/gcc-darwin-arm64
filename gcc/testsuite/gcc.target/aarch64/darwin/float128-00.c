
/* we need this for _Float128.  */
/* { dg-options "-std=gnu99 " } */
/* We use the sections anchors to make the code easier to match.  */
/* { dg-additional-options " -O2 -fsection-anchors " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* we should just pass q0 and q1 through
**foo:
**	...
**	bl	___addtf3
**	...
*/

__attribute__((__noinline__))
_Float128
foo (_Float128 a, _Float128 b)
{
  return a + b;
}


/* we should just load q0 and q1
**call_foo:
**	...
**	ldr	q1, \[x[0-9]+\]
**	...
**	ldr	q0, \[x[0-9]+\]
**	b	_foo
**	...
*/

__attribute__((__noinline__))
_Float128
call_foo (void)
{
  return foo (1.0, 2.0);
}
