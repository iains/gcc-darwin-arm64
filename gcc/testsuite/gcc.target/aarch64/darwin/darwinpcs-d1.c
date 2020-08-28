/* { dg-do compile } */
/* we need this for the empty struct.  */
/* { dg-options "-std=gnu99 " } */
/* { dg-additional-options "-O -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* Make sure we do no consume any registers in passing zero-sized entities */

typedef struct es {} Empty;

__attribute__((__noinline__)) void
use_no_regs (int a, Empty b, int c, Empty d, Empty e, int f);

/*
**call_use_no_regs:
**	...
**	mov	w2, 3
**	mov	w1, 2
**	mov	w0, 1
**	bl	_use_no_regs
**	...
*/

__attribute__((__noinline__)) void
call_use_no_regs (void)
{
  Empty e;
  use_no_regs (1, e, 2, e, e, 3);
}

/* Make sure we consume no stack in passing zero-sized entities. */

/*
**call_use_no_stack:
**	...
**	mov	w[0-9]+, 108
**	strb	w[0-9]+, \[sp, 1\]
**	mov	w[0-9]+, 106
**	strb	w[0-9]+, \[sp\]
**	...
**	bl	_use_no_stack
**	...
*/

__attribute__((__noinline__)) void
use_no_stack (int a, int b, int c, int d, int e, int f, int g, int h,
	      Empty i, char j, Empty k, char l);

void
call_use_no_stack (void)
{
  Empty e;
  use_no_stack (0, 1, 2, 3, 4, 5, 6, 7, e, 'j', e, 'l');
}
