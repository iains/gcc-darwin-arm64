/* { dg-do compile } */
/* { dg-options "-O2 -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */
/* { dg-skip-if "" { *-*-mingw* } } */

#include <stdarg.h>
int foo [[gnu::preserve_none]] (...);

/* Check preserve_none falls back to AAPCS for variadic functions. */

/* On Darwin the fallback is to darwinpcs, which passes variadic arguments on
   the stack, so none of the w1..w7 setup appears there; the *M lines assert the
   stack form rather than freezing the whole body.  Apple clang 17 does the same
   for a plain variadic call, so the difference is the ABI, not codegen.  */

/*
** bar:
** ...
*E	mov	w0, 23
*E	mov	w7, 7
*E	mov	w6, 6
*E	mov	w5, 5
*E	mov	w4, 4
*E	mov	w3, 3
*E	mov	w2, 2
*E	mov	w1, 1
*E	str	w0, \[sp, 120\]
*E	mov	w0, 22
*E	str	w0, \[sp, 112\]
*E	mov	w0, 21
*E	str	w0, \[sp, 104\]
*E	mov	w0, 20
*E	str	w0, \[sp, 96\]
*E	mov	w0, 19
*E	str	w0, \[sp, 88\]
*E	mov	w0, 18
*E	str	w0, \[sp, 80\]
*E	mov	w0, 17
*E	str	w0, \[sp, 72\]
*E	mov	w0, 16
*E	str	w0, \[sp, 64\]
*E	mov	w0, 15
*E	str	w0, \[sp, 56\]
*E	mov	w0, 14
*E	str	w0, \[sp, 48\]
*E	mov	w0, 13
*E	str	w0, \[sp, 40\]
*E	mov	w0, 12
*E	str	w0, \[sp, 32\]
*E	mov	w0, 11
*E	str	w0, \[sp, 24\]
*E	mov	w0, 10
*E	str	w0, \[sp, 16\]
*E	mov	w0, 9
*E	str	w0, \[sp, 8\]
*E	mov	w0, 8
*E	str	w0, \[sp\]
*E	mov	w0, 0
*E	bl	foo
*E	add	w0, w0, 1
** ...
*M	...
*M	str	w[0-9]+, \[sp, [0-9]+\]
*M	...
*/
int bar [[gnu::preserve_none]] ()
{
  return foo (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	      19, 20, 21, 22, 23)
	 + 1;
}
