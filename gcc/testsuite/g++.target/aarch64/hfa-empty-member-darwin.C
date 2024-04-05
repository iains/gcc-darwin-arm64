/* { dg-do compile { target *-*-darwin* } } */
/* { dg-options "-std=gnu++17 -O1" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* darwinpcs D.10c: a union member of empty class type, or a non-zero-length
   array of such, is ignored in determining whether the union is a
   homogeneous floating-point aggregate.  A member whose class merely contains
   an empty class is not ignored, and a zero-length array member still stops
   the union being an HFA (D.10a).  The expected code is what clang generates
   for arm64-apple-darwin.  */

struct X { };
struct Y { int : 0; };
union Q { X x; float a; };				/* HFA */
union R { [[no_unique_address]] X x; float a; };	/* HFA */
union UD { X x; double a; };				/* HFA */
union U2 { X x; struct { float a, b; } f; };		/* HFA */
union UA { X x[2]; float a; };				/* HFA */
union UY { Y y; float a; };				/* HFA */
union NE { struct { X x; } s; float a; };		/* not an HFA */
union Z { X x[0]; float a; };				/* not an HFA */

extern "C" {

/*
** get_Q:
**	ret
*/
float get_Q (Q q) { return q.a; }

/*
** get_R:
**	ret
*/
float get_R (R r) { return r.a; }

/*
** get_UD:
**	ret
*/
double get_UD (UD u) { return u.a; }

/*
** get_U2:
**	fmov	s0, s1
**	ret
*/
float get_U2 (U2 u) { return u.f.b; }

/*
** get_UA:
**	ret
*/
float get_UA (UA u) { return u.a; }

/*
** get_UY:
**	ret
*/
float get_UY (UY u) { return u.a; }

/*
** get_NE:
**	fmov	s0, w0
**	ret
*/
float get_NE (NE u) { return u.a; }

/*
** get_Z:
**	fmov	s0, w0
**	ret
*/
float get_Z (Z u) { return u.a; }

/* Caller side: passed in s0, not w0.
** pass_Q:
**	...
**	ldr	s0, \[x0\]
**	...
*/
float ext_Q (Q);
float pass_Q (Q *p) { return ext_Q (*p); }

}
