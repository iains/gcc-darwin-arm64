/* { dg-do compile } */

/* { dg-additional-options "-O -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

typedef short v2hi __attribute__ ((vector_size (4)));
typedef int v4si __attribute__ ((vector_size (16)));

v4si t;
int al = __alignof__ (t);

__attribute__((__noinline__)) void
v2hi_packing (v2hi a, v2hi b, v2hi c, v2hi d, v2hi e, v2hi f, v2hi g, v2hi h,
	      v2hi i, v2hi j);

/* We check that v2hi is packed on the stack.
**call_v2hi_packing:
**	...
**	mov	w[0-9]+, 1
**	movk	w[0-9]+, 0x2, lsl 16
**	str	w[0-9]+, \[sp, 4\]
**	str	w[0-9]+, \[sp\]
**	mov	w7, w[0-9]+
**	mov	w6, w[0-9]+
**	mov	w5, w[0-9]+
**	mov	w4, w[0-9]+
**	mov	w3, w[0-9]+
**	mov	w2, w[0-9]+
**	mov	w1, w[0-9]+
**	bl	_v2hi_packing
**	...
*/

void
call_v2hi_packing (void)
{
  v2hi x = {1,2};
  v2hi_packing (x, x, x, x, x, x, x, x, x, x);
}


__attribute__((__noinline__)) void
v4si_packing (int r0, int r1, int r2, int r3, int r4, int r5, int r6, int r7,
	      v4si a, v4si b, v4si c, v4si d, v4si e, v4si f, v4si g, v4si h,
	      int stack, v4si i, v4si j);

/* Test that we align a 16b vector on the stack.
**call_v4si_packing:
**	...
**	adrp	x0, lC0@PAGE
**	ldr	q[0-9]+, \[x[0-9]+, #lC0@PAGEOFF\]
**	str	q[0-9]+, \[sp, 32\]
**	str	q[0-9]+, \[sp, 16\]
**	mov	w[0-9]+, 42
**	str	w[0-9]+, \[sp\]
**	...
**	bl	_v4si_packing
**	...
*/

void
call_v4si_packing (void)
{
  v4si x = {3,1,2,4};
  v4si_packing (0, 1, 2, 3, 4, 5, 6, 7, x, x, x, x, x, x, x, x, 42, x, x);
}
