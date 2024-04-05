/* { dg-do compile } */

/* { dg-additional-options "-O2 -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

typedef union u { char a; short b; } U;
typedef struct sf { float a; float b; float c;} SF;

__attribute__((__noinline__)) void
u_packing (int a, int b, int c, int d, int e, int f, int g, int h,
	   U i, U j);

/* We check that these values are not packed on the stack.
**call_u_packing:
**	...
**	strh	w[0-9]+, \[sp, 8\]
**	strh	w[0-9]+, \[sp\]
**	...
**	bl	_u_packing
**	...
*/

void
call_u_packing (void)
{
  U x = { 'a' };
  u_packing (0, 1, 2, 3, 4, 5, 6, 7, x, x);
}

/* But a homogeneous float aggregate is treated as if it were the contained
   floats.  */

__attribute__((__noinline__)) void
sf_packing (float a, float b, float c, float d,
	    float e, float f, float g, float h,
	    SF i, SF j);

/* So the stores to sp+12 and 20 pack the floats onto the stack.
**call_sf_packing:
**	...
**	mov	(x[0-9]+), 1065353216
**	movk	\1, 0x4000, lsl 48
**	mov	w[0-9]+, 1077936128
**	fmov	s7, 7.0e\+0
**	fmov	s6, 6.0e\+0
**	fmov	s5, 5.0e\+0
**	fmov	s4, 4.0e\+0
**	fmov	s3, 3.0e\+0
**	fmov	s2, 2.0e\+0
**	fmov	s1, 1.0e\+0
**	movi	v0.2s, #0
**	str	x[0-9]+, \[sp\]
**	str	w[0-9]+, \[sp, 8\]
**	str	x[0-9]+, \[sp, 12\]
**	str	w[0-9]+, \[sp, 20\]
**	bl	_sf_packing
**	...
*/

void
call_sf_packing (void)
{
 SF A = {1.0F, 2.0F, 3.0F};
 sf_packing (0.0F, 1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F,
 	     A, A);
}
