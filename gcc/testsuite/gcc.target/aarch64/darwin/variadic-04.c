/* { dg-do compile } */
/* { dg-options "-std=gnu23" } */
/* { dg-additional-options "-O2 -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* The first unnamed argument of a variadic call starts at the next stack
   slot after packed named stack arguments; the offsets below match clang.
   variadic-03.c checks caller/callee agreement by execution; this pins the
   placement itself.  */

#include <stdarg.h>

#define L8 long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7
#define LV 1, 2, 3, 4, 5, 6, 7, 8

/* Last named arg packed on the stack at offset 1: the unnamed long is at the
   incoming sp+8.
**f2:
**	sub	sp, sp, #16
**	...
**	ldr	x0, \[sp, 24\]
**	...
*/

__attribute__((__noinline__)) long
f2 (L8, char p, char q, ...)
{
  va_list ap;
  va_start (ap);
  long r = va_arg (ap, long);
  va_end (ap);
  return r;
}

/*
**call_f2:
**	...
**	str	x[0-9]+, \[sp, 8\]
**	...
**	strb	w[0-9]+, \[sp, 1\]
**	...
**	strb	w[0-9]+, \[sp\]
**	...
**	bl	_f2
**	...
*/

__attribute__((__noinline__)) long
call_f2 (void)
{
  return f2 (LV, 11, 12, 42L);
}

/* The last named arg is in a register, but an earlier one is packed on the
   stack: the unnamed long is still at the incoming sp+8.
**fr:
**	sub	sp, sp, #16
**	...
**	ldr	x0, \[sp, 24\]
**	...
*/

__attribute__((__noinline__)) long
fr (L8, char p, double d, ...)
{
  va_list ap;
  va_start (ap);
  long r = va_arg (ap, long);
  va_end (ap);
  return r;
}

/*
**call_fr:
**	...
**	str	x[0-9]+, \[sp, 8\]
**	...
**	strb	w[0-9]+, \[sp\]
**	...
**	bl	_fr
**	...
*/

__attribute__((__noinline__)) long
call_fr (void)
{
  return fr (LV, 11, 2.5, 42L);
}
