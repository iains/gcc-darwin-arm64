/* { dg-do compile } */

/* we need this for _Float128.  */
/* { dg-options "-std=gnu99 " } */
/* We use the sections anchors to make the code easier to match.  */
/* { dg-additional-options " -O2 -fsection-anchors " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

#include <stdarg.h>

/* What we care about here is that q0 and q1 are loaded from incoming sp and
   sp+16.
**foo:
**	...
**	ldr	q1, \[x29, 32\]
**	...
**	ldr	q0, \[x29, 48\]
**	...
**	bl	___addtf3
**	...
*/

__attribute__((__noinline__))
_Float128
foo (int n, ...)
{
  _Float128 a, b;
  va_list ap;

  va_start(ap, n);
  a = va_arg(ap, _Float128);
  b = va_arg(ap, _Float128);
  va_end(ap); 
  return a + b;
}

/*
**call_foo:
**	...
**	mov	w0, 2
**	str	q[0-9]+, \[sp, 16\]
**	str	q[0-9]+, \[sp\]
**	bl	_foo
**	...
*/

__attribute__((__noinline__))
_Float128
call_foo (void)
{
  return foo (2, (_Float128)1.0, (_Float128)2.0);
}

/* What we care about here is that q0 and q1 are loaded from incoming sp and
   sp+32 (with the int at sp+16).
**bar:
**	...
**	ldr	w[0-9]+, \[x[0-9]+, 16\]
**	...
**	ldr	q0, \[x[0-9]+\]
**	...
**	ldr	q1, \[x[0-9]+, 32\]
**	bl	___addtf3
**	...
*/

__attribute__((__noinline__))
_Float128
bar (int n, ...)
{
  _Float128 a, b;
  va_list ap;

  va_start(ap, n);
  a = va_arg(ap, _Float128);
  n = va_arg(ap, int);
  if (n != 42)
    __builtin_abort ();
  b = va_arg(ap, _Float128);
  va_end(ap); 
  return a + b;
}

/*
**call_bar:
**	...
**	mov	(w[0-9]+), 42
**	...
**	str	\1, \[sp, 16\]
**	...
**	mov	w0, 2
**	str	q[0-9]+, \[sp, 32\]
**	str	q[0-9]+, \[sp\]
**	bl	_bar
**	...
*/

__attribute__((__noinline__))
_Float128
call_bar (void)
{
  return bar (2, (_Float128)1.0,
	      42, (_Float128)2.0);
}
