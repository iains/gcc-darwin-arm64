/* { dg-do compile } */
/* { dg-options "-std=gnu11 -O1" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* darwinpcs D.14 (clang; not in the written spec): in determining whether an
   aggregate is a homogeneous floating-point aggregate, __bf16 is a
   floating-point base type, and the same base type as _Float16.  The register
   assignments are those clang uses for arm64-apple-darwin.  */

struct B2 { __bf16 a, b; };		/* HFA */
struct BA { __bf16 a[3]; };		/* HFA */
struct B4 { __bf16 a, b, c, d; };	/* HFA */
struct B5 { __bf16 a, b, c, d, e; };	/* not an HFA: five members */
struct BH { __bf16 a; _Float16 b; };	/* HFA */
struct BF { __bf16 a; float b; };	/* not an HFA */
union  UB { __bf16 a; __bf16 b[2]; };	/* HFA */

/* The second member arrives in h1.
** get_B2:
**	...
**	umov	w[0-9]+, v1.h\[0\]
**	...
**	ret
*/
__bf16 get_B2 (struct B2 s) { return s.b; }

/*
** get_BA:
**	...
**	mov	v0.h\[0\], v2.h\[0\]
**	...
**	ret
*/
__bf16 get_BA (struct BA s) { return s.a[2]; }

/*
** get_B4:
**	mov	v0.h\[0\], v3.h\[0\]
**	ret
*/
__bf16 get_B4 (struct B4 s) { return s.d; }

/* Not an HFA: in x0/x1, the fifth member in the low half of x1.
** get_B5:
**	...
**	strh	w1, .*
**	...
**	ret
*/
__bf16 get_B5 (struct B5 s) { return s.e; }

/*
** get_BH:
**	...
**	umov	w[0-9]+, v1.h\[0\]
**	...
**	ret
*/
_Float16 get_BH (struct BH s) { return s.b; }

/* Not an HFA: in x0.
** get_BF:
**	lsr	x0, x0, 32
**	fmov	s0, w0
**	ret
*/
float get_BF (struct BF s) { return s.b; }

/*
** get_UB:
**	...
**	umov	w[0-9]+, v1.h\[0\]
**	...
**	ret
*/
__bf16 get_UB (union UB s) { return s.b[1]; }

/* Returned in h0/h1.
** ret_B2:
**	...
**	dup	v0.4h, w1
**	dup	v1.4h, w0
**	ret
*/
struct B2 ret_B2 (__bf16 x, __bf16 y) { struct B2 s = { x, y }; return s; }

/*
** pass_B2:
**	...
**	ldr	h0, \[x0\]
**	ldr	h1, \[x0, 2\]
**	bl	_ext_B2
**	...
*/
__bf16 ext_B2 (struct B2);
__bf16 pass_B2 (struct B2 *p) { return ext_B2 (*p); }

/*
** pass_B4:
**	...
**	ldr	h0, \[x0\]
**	ldr	h1, \[x0, 2\]
**	ldr	h2, \[x0, 4\]
**	ldr	h3, \[x0, 6\]
**	bl	_ext_B4
**	...
*/
__bf16 ext_B4 (struct B4);
__bf16 pass_B4 (struct B4 *p) { return ext_B4 (*p); }
