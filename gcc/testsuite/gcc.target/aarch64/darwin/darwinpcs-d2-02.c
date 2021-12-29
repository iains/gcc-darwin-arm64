/* { dg-do compile } */
/* we need this for complex literals.  */
/* { dg-options "-std=gnu99 " } */
/* { dg-additional-options "-O -fsection-anchors -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */


__attribute__((__noinline__)) void
c_cc_packing (int a, int b, int c, int d, int e, int f, int g, int h,
	      _Complex char i, _Complex char j);

/* We check that these values are not packed on the stack.
**call_c_cc_packing:
**	...
**	ldrh	w[0-9]+, \[x[0-9]+\]
**	strh	w[0-9]+, \[sp, 8\]
**	ldrh	w[0-9]+, \[x[0-9]+, 2\]
**	strh	w[0-9]+, \[sp\]
**	...
**	bl	_c_cc_packing
**	...
*/

void
call_c_cc_packing (void)
{
 c_cc_packing (0, 1, 2, 3, 4, 5, 6, 7,
	       (_Complex char) (1 + 1i),(_Complex char) (2 + 2i));
}


__attribute__((__noinline__)) void
c_cs_packing (int a, int b, int c, int d, int e, int f, int g, int h,
		   _Complex short i, _Complex short j);

/*
**call_c_cs_packing:
**	...
**	ldr	w[0-9]+, \[x[0-9]+, 4\]
**	str	w[0-9]+, \[sp, 8\]
**	ldr	w[0-9]+, \[x[0-9]+, 8\]
**	str	w[0-9]+, \[sp\]
**	...
**	bl	_c_cs_packing
**	...
*/

void
call_c_cs_packing (void)
{
 c_cs_packing (0, 1, 2, 3, 4, 5, 6, 7,
	       (_Complex short) (1 + 1i),(_Complex short) (2 + 2i));
}

void c_ci_packing (int a, int b, int c, int d, int e, int f, int g, int h,
		   _Complex int i, _Complex int j);

/*
**call_c_ci_packing:
**	...
**	ldr	x[0-9]+, \[x[0-9]+, 12\]
**	str	x[0-9]+, \[sp, 8\]
**	ldr	x[0-9]+, \[x[0-9]+, 20\]
**	str	x[0-9]+, \[sp\]
**	...
**	bl	_c_ci_packing
**	...
*/

void
call_c_ci_packing (void)
{
 c_ci_packing (0, 1, 2, 3, 4, 5, 6, 7,
	       (_Complex int) (1 + 1i),(_Complex int) (2 + 2i));
}
