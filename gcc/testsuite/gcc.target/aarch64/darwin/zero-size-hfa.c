/* { dg-do compile } */
/* { dg-options "-std=gnu11 -O1" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* darwinpcs D.10a (clang; not in the written spec): an aggregate with a
   zero-length array member is not a homogeneous floating-point aggregate,
   wherever the member is and whatever its element type, and this applies to
   unions too.  An empty struct member does not change this
   (D.10b).  The expected code is what clang generates for arm64-apple-darwin.  */

struct H  { unsigned char b[0]; _Complex double c; };	/* not an HFA */
struct T  { _Complex double c; unsigned char b[0]; };	/* not an HFA */
struct HF { int b[0]; _Complex float c; };		/* not an HFA */
struct P  { _Complex double c; };			/* HFA */
struct E  { struct {} a; _Complex double c; };		/* HFA */
struct ZF { float a[0]; float c; };			/* not an HFA */
struct FZ { float c; float a[0]; };			/* not an HFA */
union UZF { float a[0]; float c; };			/* not an HFA */

/*
** re_H:
**	fmov	d0, x0
**	ret
*/
double re_H (struct H s) { return __real__ s.c; }

/*
** re_T:
**	fmov	d0, x0
**	ret
*/
double re_T (struct T s) { return __real__ s.c; }

/*
** re_HF:
**	fmov	s0, w0
**	ret
*/
float re_HF (struct HF s) { return __real__ s.c; }

/*
** re_P:
**	ret
*/
double re_P (struct P s) { return __real__ s.c; }

/*
** re_E:
**	ret
*/
double re_E (struct E s) { return __real__ s.c; }

/* Caller side: passed in x0/x1, not d0/d1.
** pass_H:
**	...
**	ldp	x0, x1, \[x0\]
**	bl	_ext_H
**	...
*/
double ext_H (struct H);
double pass_H (struct H *p) { return ext_H (*p); }

/* Returned in x0/x1, not d0/d1.
** use_ret_H:
**	...
**	bl	_get_H
**	fmov	d0, x0
**	...
*/
struct H get_H (void);
double use_ret_H (void) { return __real__ get_H ().c; }

/*
** get_ZF:
**	fmov	s0, w0
**	ret
*/
float get_ZF (struct ZF s) { return s.c; }

/*
** get_FZ:
**	fmov	s0, w0
**	ret
*/
float get_FZ (struct FZ s) { return s.c; }

/*
** get_UZF:
**	fmov	s0, w0
**	ret
*/
float get_UZF (union UZF s) { return s.c; }
