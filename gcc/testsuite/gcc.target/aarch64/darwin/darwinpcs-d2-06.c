/* { dg-do compile } */
/* { dg-options "-std=gnu99 -O -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* Complex, union and non-homogeneous struct arguments are not packed on the
   stack: each occupies whole 8-byte slots, so a following char starts the
   next slot (as clang does).  */

struct s1 { char x; };
union u { char c; short s; };

void fc (int, int, int, int, int, int, int, int, _Complex char, char);
void fs (int, int, int, int, int, int, int, int, struct s1, char);
void fu (int, int, int, int, int, int, int, int, union u, char);

/*
**gc:
**	...
**	strb	w[0-9]+, \[sp, 8\]
**	...
**	bl	_fc
**	...
*/
void gc (_Complex char z) { fc (0, 1, 2, 3, 4, 5, 6, 7, z, 'c'); }

/*
**gs:
**	...
**	strb	w[0-9]+, \[sp, 8\]
**	...
**	bl	_fs
**	...
*/
void gs (struct s1 s) { fs (0, 1, 2, 3, 4, 5, 6, 7, s, 'c'); }

/*
**gu:
**	...
**	strb	w[0-9]+, \[sp, 8\]
**	...
**	bl	_fu
**	...
*/
void gu (union u u) { fu (0, 1, 2, 3, 4, 5, 6, 7, u, 'c'); }
