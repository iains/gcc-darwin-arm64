/* { dg-do compile } */

/* we need this for __int128.  */
/* { dg-options "-std=gnu99 " } */
/* We use the sections anchors to make the code easier to match.  */
/* { dg-additional-options " -O2 -fsection-anchors " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

#include <stdarg.h>

/* What we care about here is that we load the values from incoming sp and
   sp + 16.
**foo:
**	sub	sp, sp, #16
**	...
**	ldp	x[0-9]+, x[0-9]+, \[sp, 16\]
**	...
**	ldr	x[0-9]+, \[sp, 32\]
**	ldr	x[0-9]+, \[sp, 40\]
**	...
*/

__attribute__((__noinline__))
__int128
foo (int n, ...)
{
  __int128 a, b;
  va_list ap;

  va_start(ap, n);
  a = va_arg(ap, __int128);
  b = va_arg(ap, __int128);
  va_end(ap); 
  return a + b;
}

/*
**call_foo:
**	...
**	mov	w0, 2
**	stp	x[0-9]+, x[0-9]+, \[sp\]
**	stp	x[0-9]+, x[0-9]+, \[sp, 16\]
**	bl	_foo
**	...
*/

__attribute__((__noinline__))
__int128
call_foo (void)
{
  return foo (2, (__int128)1, (__int128)2);
}


/* sp = one int128, sp+16 = int sp + 32 = other int128 
**bar:
**	...
**	sub	sp, sp, #16
**	...
**	ldp	x[0-9]+, x[0-9]+, \[sp, 16\]
**	...
**	ldr	x[0-9]+, \[sp, 48\]
**	ldr	x[0-9]+, \[sp, 56\]
**	...
*/

__attribute__((__noinline__))
__int128
bar (int n, ...)
{
  __int128 a, b;
  va_list ap;

  va_start(ap, n);
  a = va_arg(ap, __int128);
  n = va_arg(ap, int);
  b = va_arg(ap, __int128);
  va_end(ap); 
  return a + b;
}

__attribute__((__noinline__))
__int128
baro (int n, ...);

/*
**call_bar:
**	...
**	mov	w[0-9]+, 42
**	...
**	mov	w0, 2
**	stp	x[0-9]+, x[0-9]+, \[sp\]
**	str	w[0-9]+, \[sp, 16\]
**	stp	x[0-9]+, x[0-9]+, \[sp, 32\]
**	bl	_baro
**	...
*/

__attribute__((__noinline__))
__int128
call_bar (void)
{
  return baro (2, (__int128)1, 42, (__int128)2);
}
