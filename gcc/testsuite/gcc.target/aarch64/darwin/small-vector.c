/* { dg-do compile } */
/* { dg-options "-std=gnu11 -O2" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */
/* Float-element vector arguments are never read from the stack.  */
/* { dg-final { scan-assembler-not {ldrh?\tw1, \[sp\]} } } */

/* darwinpcs D.11: a generic vector smaller than 8 bytes is passed as a 32-bit
   integer (in a W register, or in a 4-byte, 4-aligned stack slot).
   darwinpcs D.8: one-element and floating-point ones are returned in the low
   part of v0.  The expected code matches clang for arm64-apple-darwin.  */

typedef char  c1 __attribute__((vector_size (1)));
typedef char  c2 __attribute__((vector_size (2)));
typedef short s1 __attribute__((vector_size (2)));
typedef int   i1 __attribute__((vector_size (4)));
typedef float f1 __attribute__((vector_size (4)));
typedef _Float16 h1 __attribute__((vector_size (2)));
typedef _Float16 h2 __attribute__((vector_size (4)));
#define L long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7
#define F double d0, double d1, double d2, double d3, double d4, double d5, \
	  double d6, double d7

/* Float-element vectors are passed in W registers (D.11).  */

/*
** a_f1:
**	...
**	str	w1, \[x0\]
**	...
*/
void a_f1 (f1 *p, f1 v) { *p = v; }

/*
** a_h1:
**	...
**	strh	w1, \[x0\]
**	...
*/
void a_h1 (h1 *p, h1 v) { *p = v; }

/*
** a_h2:
**	str	w1, \[x0\]
**	ret
*/
void a_h2 (h2 *p, h2 v) { *p = v; }

/*
** c_f1:
**	...
**	ldr	w1, \[x0\]
**	...
*/
void ef1 (int, f1);
void c_f1 (f1 *p) { ef1 (0, *p); }

/*
** c_h1:
**	...
**	ldrh	w1, \[x0\]
**	...
*/
void eh1 (int, h1);
void c_h1 (h1 *p) { eh1 (0, *p); }

/*
** c_h2:
**	...
**	ldr	w1, \[x0\]
**	...
*/
void eh2 (int, h2);
void c_h2 (h2 *p) { eh2 (0, *p); }

/* On the stack, each takes a 4-byte, 4-aligned slot (D.11).  */

/*
** k_c1:
**	ldrsb	w0, \[sp, 8\]
**	ret
*/
char k_c1 (L, F, char k, c1 v, char k2) { return k2; }

/*
** k_c2:
**	ldrsb	w0, \[sp, 8\]
**	ret
*/
char k_c2 (L, F, char k, c2 v, char k2) { return k2; }

/*
** k_h1:
**	ldrsb	w0, \[sp, 8\]
**	ret
*/
char k_h1 (L, F, char k, h1 v, char k2) { return k2; }

/*
** v_c1:
**	...
**	ldrb	w[0-9]+, \[sp, 4\]
**	...
*/
void v_c1 (L, F, char k, c1 v, char k2, c1 *p) { *p = v; }

/*
** v_c2:
**	...
**	ldrh	w[0-9]+, \[sp, 4\]
**	...
*/
void v_c2 (L, F, char k, c2 v, char k2, c2 *p) { *p = v; }

/* One-element and floating-point vectors are returned in v0 (D.8).  */

/*
** r_c1:
**	ldr	b0, \[x0\]
**	ret
*/
c1 r_c1 (c1 *p) { return *p; }

/*
** r_s1:
**	ldr	h0, \[x0\]
**	ret
*/
s1 r_s1 (s1 *p) { return *p; }

/*
** r_i1:
**	ldr	s0, \[x0\]
**	ret
*/
i1 r_i1 (i1 *p) { return *p; }

/*
** r_f1:
**	...
**	ldr	s0, \[x0\]
**	...
*/
f1 r_f1 (f1 *p) { return *p; }

/*
** r_h1:
**	...
**	ldr	h0, \[x0\]
**	...
*/
h1 r_h1 (h1 *p) { return *p; }

/*
** r_h2:
**	ldr	s0, \[x0\]
**	ret
*/
h2 r_h2 (h2 *p) { return *p; }
