/* { dg-do compile } */
/* { dg-additional-options " -O -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* In each case we consume the parm registers with 8 ints, forcing
   the test values to be spilled to the stack.  */

/* The important thing here is that the chars are assigned to the stack
 * with no padding - so that they occupy bytes 0-8. */

/*
**call_char_packing:
**	...
**	mov	w[0-9]+, 113
**	strb	w[0-9]+, \[sp, 8\]
**	mov	w[0-9]+, 112
**	strb	w[0-9]+, \[sp, 7\]
**	mov	w[0-9]+, 111
**	strb	w[0-9]+, \[sp, 6\]
**	mov	w[0-9]+, 110
**	strb	w[0-9]+, \[sp, 5\]
**	mov	w[0-9]+, 109
**	strb	w[0-9]+, \[sp, 4\]
**	mov	w[0-9]+, 108
**	strb	w[0-9]+, \[sp, 3\]
**	mov	w[0-9]+, 107
**	strb	w[0-9]+, \[sp, 2\]
**	mov	w[0-9]+, 106
**	strb	w[0-9]+, \[sp, 1\]
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
**	bl	_char_packing
**	...
*/

__attribute__((__noinline__)) void
char_packing (int a, int b, int c, int d, int e, int f, int g, int h,
	      char i, char j, char k, char l,
	      char m, char n, char o, char p,
	      char q);

void call_char_packing (void)
{
 char_packing (0, 1, 2, 3, 4, 5, 6, 7,
	       'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q');
}

/* Here we should occupy the first 7 short words on the stack. */

/*
**call_short_packing:
**	...
**	mov	w[0-9]+, 12
**	strh	w[0-9]+, \[sp, 8\]
**	mov	w[0-9]+, 11
**	strh	w[0-9]+, \[sp, 6\]
**	mov	w[0-9]+, 10
**	strh	w[0-9]+, \[sp, 4\]
**	mov	w[0-9]+, 9
**	strh	w[0-9]+, \[sp, 2\]
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
**	bl	_short_packing
**	...
*/

__attribute__((__noinline__)) void
short_packing (int a, int b, int c, int d, int e, int f, int g, int h,
		    short i, short j, short k, short l,
		    short m);

void call_short_packing (void)
{
 short_packing (0, 1, 2, 3, 4, 5, 6, 7,
	        8, 9, 10, 11, 12);
}

/* Here we should occupy the first 3 ints on the stack. */

/*
**call_int_packing:
**	...
**	mov	w[0-9]+, 10
**	str	w[0-9]+, \[sp, 8\]
**	mov	w[0-9]+, 9
**	mov	w[0-9]+, 8
**	stp	w[0-9]+, w[0-9]+, \[sp\]
**	mov	w7, 7
**	mov	w6, 6
**	mov	w5, 5
**	mov	w4, 4
**	mov	w3, 3
**	mov	w2, 2
**	mov	w1, 1
**	mov	w0, 0
**	bl	_int_packing
**	...
*/

__attribute__((__noinline__)) void
int_packing (int a, int b, int c, int d, int e, int f, int g, int h,
	     int i, int j, int k);

void call_int_packing (void)
{
 int_packing (0, 1, 2, 3, 4, 5, 6, 7,
	      8, 9, 10);
}

