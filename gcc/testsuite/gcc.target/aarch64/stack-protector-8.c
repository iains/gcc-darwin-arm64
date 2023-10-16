/* { dg-options " -O -fstack-protector-strong -mstack-protector-guard=sysreg -mstack-protector-guard-reg=tpidr2_el0 -mstack-protector-guard-offset=16" } */
/* { dg-final { check-function-bodies {"**" "*E"} "" "" { target { ! *-*-darwin* } } } } */
/* { dg-final { check-function-bodies {"**" "*M"} "" "" { target *-*-darwin* } } } */

void g(void *);
__SVBool_t *h(void *);

/* On Darwin the frame pointer is required (darwinpcs D.12), so the frame
   is addressed from x29 rather than sp (the *M lines).
** test1:
**	sub	sp, sp, #288
**	stp	x29, x30, \[sp, #?272\]
**	add	x29, sp, #?272
**	mrs	(x[0-9]+), tpidr2_el0
**	ldr	(x[0-9]+), \[\1, #?16\]
*E	str	\2, \[sp, #?264\]
*M	str	\2, \[x29, #?-8\]
**	mov	\2, #?0
*E	add	x0, sp, #?8
*M	sub	x0, x29, #?264
**	bl	_?g
**	...
**	mrs	.*
**	...
**	bne	.*
**	...
**	ldp	x29, x30, \[sp, #?272\]
**	add	sp, sp, #?288
**	ret
**	bl	_?__stack_chk_fail
*/
int test1() {
  int y[0x40];
  g(y);
  return 1;
}

/*
** test2:
**	stp	x29, x30, \[sp, #?-16\]!
**	mov	x29, sp
**	sub	sp, sp, #1040
**	mrs	(x[0-9]+), tpidr2_el0
**	ldr	(x[0-9]+), \[\1, #?16\]
*E	str	\2, \[sp, #?1032\]
*M	str	\2, \[x29, #?-8\]
**	mov	\2, #?0
*E	add	x0, sp, #?8
*M	sub	x0, x29, #?1032
**	bl	_?g
**	...
**	mrs	.*
**	...
**	bne	.*
**	...
*E	add	sp, sp, #?1040
*M	mov	sp, x29
**	ldp	x29, x30, \[sp\], #?16
**	ret
**	bl	_?__stack_chk_fail
*/
int test2() {
  int y[0x100];
  g(y);
  return 1;
}

#pragma GCC target "+sve"

/*
** test3:
**	stp	x29, x30, \[sp, #?-16\]!
**	mov	x29, sp
**	addvl	sp, sp, #-18
**	...
**	str	p4, \[sp\]
**	...
**	sub	sp, sp, #272
**	mrs	(x[0-9]+), tpidr2_el0
*E	ldr	(x[0-9]+), \[\1, #?16\]
*E	str	\2, \[sp, #?264\]
*E	mov	\2, #?0
*E	add	x0, sp, #?8
*M	addvl	(x[0-9]+), x29, #-18
*M	sub	\2, \2, #8
*M	ldr	(x[0-9]+), \[\1, #?16\]
*M	str	\3, \[\2\]
*M	mov	\3, #?0
*M	addvl	x0, x29, #-18
*M	sub	x0, x0, #264
**	bl	_?h
**	...
**	mrs	.*
**	...
**	bne	.*
*E	...
*E	add	sp, sp, #?272
*M	addvl	sp, x29, #-18
**	...
**	ldr	p4, \[sp\]
**	...
**	addvl	sp, sp, #18
**	ldp	x29, x30, \[sp\], #?16
**	ret
**	bl	_?__stack_chk_fail
*/
__SVBool_t test3() {
  int y[0x40];
  return *h(y);
}
