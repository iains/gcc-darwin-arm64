/* { dg-do compile } */
/* { dg-options "-std=gnu11 -O1" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/* darwinpcs D.15b (clang; not in the written spec): complex floating-point
   arguments on the stack are packed like the equivalent homogeneous aggregate,
   at their natural alignment, rather than starting a new 8-byte slot as
   complex integer types do (D.15a, see darwinpcs-d2-06.c).  The offsets are
   those clang uses for arm64-apple-darwin.  The eight leading arguments fill
   the argument registers.  */

#define D8 double d0, double d1, double d2, double d3, \
	   double d4, double d5, double d6, double d7
#define I8 int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7

/* x at sp+0, c at sp+4.
** re_cf:
**	ldr	s0, \[sp, 4\]
**	ret
*/
float re_cf (D8, float x, _Complex float c) { return __real__ c; }

/*
** im_cf:
**	ldr	s0, \[sp, 8\]
**	ret
*/
float im_cf (D8, float x, _Complex float c) { return __imag__ c; }

/* x at sp+0, c at sp+2.
** re_ch:
**	ldr	h0, \[sp, 2\]
**	ret
*/
_Float16 re_ch (D8, _Float16 x, _Complex _Float16 c) { return __real__ c; }

/*
** im_ch:
**	ldr	h0, \[sp, 4\]
**	ret
*/
_Float16 im_ch (D8, _Float16 x, _Complex _Float16 c) { return __imag__ c; }

/* Natural alignment 8: c at sp+8.
** re_cd:
**	ldr	d0, \[sp, 8\]
**	ret
*/
double re_cd (D8, float x, _Complex double c) { return __real__ c; }

/* c at sp+0, y packed after it at sp+8.
** after_cf:
**	ldr	s0, \[sp, 8\]
**	ret
*/
float after_cf (D8, _Complex float c, float y) { return y; }

/* Control (D.15a): a complex integer type starts a new slot, c at sp+8.
** im_cc:
**	ldrsb	w0, \[sp, 9\]
**	ret
*/
char im_cc (I8, char x, _Complex char c) { return __imag__ c; }
