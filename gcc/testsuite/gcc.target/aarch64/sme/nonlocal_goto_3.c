/* { dg-options "-O2 -fno-schedule-insns -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" } } */

void run(void (*)());

/* Darwin uses heap trampolines: taking the address of a nested function calls
   __gcc_nested_func_ptr_created, and a matching __gcc_nested_func_ptr_deleted
   tears it down, where the stack-trampoline implementation instead builds the
   trampoline inline and flushes the cache.  The two are different enough to be
   worth a body of their own rather than interleaved variants.
   The asymmetry is a platform constraint, not a preference: Darwin enforces a
   no-executable-stack policy, so a stack trampoline cannot work on aarch64 at
   all, and warns at link time on x86 from macOS 11.  -ftrampoline-impl= is
   generic and heap trampolines are available on ELF too, but they are not the
   default there, so the *E body is left describing that default.  */
/*
** foo:
*E	...
*E	smstart	sm
*E	...
*E	smstop	sm
*E	bl	[^\n]*[cC]ache[^\n]*
*E	smstart	sm
*E	add	x0, .*
*E	smstop	sm
*E	bl	_?run
*E	smstart	sm
*E	mov	w0, 1
*E	...
*E	smstart	sm
*E	mov	w0, 0
*E	smstop	sm
*E	...
*M	...
*M	smstart	sm
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
__arm_locally_streaming int
foo (int *ptr)
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
