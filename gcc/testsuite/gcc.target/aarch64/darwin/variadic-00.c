/* { dg-do compile } */

/* We use the sections anchors to make the code easier to match.  */
/* { dg-additional-options " -O -fsection-anchors -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

#include <stdarg.h>

/* What we care about here is that the load of w0 is from the incoming [SP]
**fooi:
**	sub	sp, sp, #16
**	add	x[0-9]+, sp, 24
**	str	x[0-9]+, \[sp, 8\]
**	ldr	w0, \[sp, 16\]
**	...
*/

__attribute__((__noinline__)) int
fooi (int a, ...)
{
  int x;
  va_list ap;
  va_start(ap, a);
  x = va_arg(ap, int);
  va_end(ap);
  return x;
}

__attribute__((__noinline__)) int
fooo (char a, ...);

/*
**call_foo:
**	...
**	mov	w[0-9]+, 42
**	str	w[0-9]+, \[sp\]
**	mov	w0, 1
**	bl	_fooo
**	...
*/

__attribute__((__noinline__)) int
call_foo (void)
{
  int y = fooo (1, 42);
  return y;
}

/* What we care about here is that the load of w0 is from the incoming [SP+8]
**bari:
**	sub	sp, sp, #16
**	add	x[0-9]+, sp, 32
**	str	x[0-9]+, \[sp, 8\]
**	ldr	w0, \[sp, 24\]
**	...
*/

__attribute__((__noinline__)) int
bari (int a, int b, int c, int d, int e, int f, int g, int h,
      int i, ...)
{
  int x;
  va_list ap;
  va_start(ap, i);
  x = va_arg(ap, int);
  va_end(ap);
  return x;
}

/*
**call_bar:
**	...
**	mov	w[0-9]+, 42
**	str	w[0-9]+, \[sp, 8\]
**	mov	w[0-9]+, 9
**	str	w[0-9]+, \[sp\]
**	...
	bl	_baro
**	...
*/

__attribute__((__noinline__)) int
baro (int a, int b, int c, int d, int e, int f, int g, int h,
      int i, ...);

__attribute__((__noinline__)) int
call_bar (void)
{
  int y = baro (1, 2, 3, 4, 5, 6, 7, 8, 9, 42);
  return y;
}
