/* { dg-options "-O2 -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies {"**" "*E"} "" "" { target { ! *-*-darwin* } } } } */
/* { dg-final { check-function-bodies {"**" "*M"} "" "" { target *-*-darwin* } } } */

void run(void (*)());

/* Darwin uses heap trampolines (__gcc_nested_func_ptr_created/_deleted)
   instead of building one on the stack, so the two get separate bodies.  */
/*
** foo:
*E	...
*E	smstop	sm
*E	bl	[^\n]*[cC]ache[^\n]*
*E	smstart	sm
*E	add	x0, .*
*E	smstop	sm
*E	bl	run
*E	smstart	sm
*E	mov	w0, 1
*E	...
*E	ret
*E	smstart	sm
*E	mov	w0, 0
*E	...
*M	...
*M	smstop	sm
*M	bl	_?__gcc_nested_func_ptr_created
*M	smstart	sm
*M	ldr	x0, .*
*M	smstop	sm
*M	bl	_?run
*M	smstart	sm
*M	mov	w0, 1
*M	...
*M	smstop	sm
*M	bl	_?__gcc_nested_func_ptr_deleted
*M	smstart	sm
*M	...
*M	ret
*M	smstart	sm
*M	str	wzr, .*
*M	...
*/
int
foo (int *ptr) __arm_streaming
{
  __label__ failure;

  void bar () { *ptr += 1; goto failure; }
  run (bar);
  return 1;

failure:
  return 0;
}

// { dg-final { scan-assembler {\tstp\tx19, x20,} } }
// { dg-final { scan-assembler {\tstp\tx21, x22,} } }
// { dg-final { scan-assembler {\tstp\tx23, x24,} } }
// { dg-final { scan-assembler {\tstp\tx25, x26,} } }
// { dg-final { scan-assembler {\tstp\tx27, x28,} } }
// { dg-final { scan-assembler {\tstp\td8, d9,} } }
// { dg-final { scan-assembler {\tstp\td10, d11,} } }
// { dg-final { scan-assembler {\tstp\td12, d13,} } }
// { dg-final { scan-assembler {\tstp\td14, d15,} } }
