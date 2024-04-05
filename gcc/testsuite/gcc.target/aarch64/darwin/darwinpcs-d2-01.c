/* { dg-do compile } */
/* { dg-additional-options " -O -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* In each case we consume the parm registers with 8 ints, forcing
   the test values to be spilled to the stack.  */

/* char short char short - everything on 2byte boundaries */

/*
**call_c_s_packing:
**	...
**	mov	w[0-9]+, 109
**	strb	w[0-9]+, \[sp, 8\]
**	mov	w[0-9]+, 9
**	strh	w[0-9]+, \[sp, 6\]
**	mov	w[0-9]+, 107
**	strb	w[0-9]+, \[sp, 4\]
**	mov	w[0-9]+, 8
**	strh	w[0-9]+, \[sp, 2\]
**	mov	w[0-9]+, 105
**	strb	w[0-9]+, \[sp\]
**	mov	w7, 7
**	mov	w6, 6
**	mov	w5, 5
**	mov	w4, 4
**	mov	w3, 3
**	mov	w2, 2
**	mov	w1, 1
**	mov	w0, 0
**	bl	_c_s_packing
**	...
*/

__attribute__((__noinline__)) void
c_s_packing (int a, int b, int c, int d, int e, int f, int g, int h,
	     char i, short j, char k, short l,
	     char m);

void call_c_s_packing (void)
{
 c_s_packing (0, 1, 2, 3, 4, 5, 6, 7,
	      'i', 8 , 'k', 9, 'm');
}

/*
**call_s_c_packing:
**	...
**	mov	w[0-9]+, 109
**	strb	w[0-9]+, \[sp, 7\]
**	mov	w[0-9]+, 108
**	strb	w[0-9]+, \[sp, 6\]
**	mov	w[0-9]+, 9
**	strh	w[0-9]+, \[sp, 4\]
**	mov	w[0-9]+, 106
**	strb	w[0-9]+, \[sp, 2\]
**	mov	w[0-9]+, 8
**	strh	w[0-9]+, \[sp\]
**	mov	w7, 7
**	mov	w6, 6
**	mov	w5, 5
**	mov	w4, 4
**	mov	w3, 3
**	mov	w2, 2
**	mov	w1, 1
**	mov	w0, 0
**	bl	_s_c_packing
**	...
*/

__attribute__((__noinline__)) void
s_c_packing (int a, int b, int c, int d, int e, int f, int g, int h,
	     short i, char j, short k, char l,
	     char m);

void call_s_c_packing (void)
{
 s_c_packing (0, 1, 2, 3, 4, 5, 6, 7,
	      8, 'j' , 9, 'l', 'm');
}

/* 0, 2, 4, 0 */

/*
**call_csi_packing:
**	...
**	mov	w[0-9]+, 108
**	strb	w[0-9]+, \[sp, 8\]
**	mov	w[0-9]+, 9
**	str	w[0-9]+, \[sp, 4\]
**	mov	w[0-9]+, 8
**	strh	w[0-9]+, \[sp, 2\]
**	mov	w[0-9]+, 105
**	strb	w[0-9]+, \[sp\]
**	mov	w7, 7
**	mov	w6, 6
**	mov	w5, 5
**	mov	w4, 4
**	mov	w3, 3
**	mov	w2, 2
**	mov	w1, 1
**	mov	w0, 0
**	bl	_csi_packing
**	...
*/

__attribute__((__noinline__)) void
csi_packing (int a, int b, int c, int d, int e, int f, int g, int h,
	     char i, short j, int k, char l);

void call_csi_packing (void)
{
 csi_packing (0, 1, 2, 3, 4, 5, 6, 7,
	      'i', 8 , 9, 'l');
}
