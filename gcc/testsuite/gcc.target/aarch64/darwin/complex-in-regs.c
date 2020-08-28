/* { dg-do compile } */
/* we need this for complex and gnu initializers.  */
/* { dg-options "-std=gnu99 " } */
/* We use the sections anchors to make the code easier to match.  */
/* { dg-additional-options " -O -fsection-anchors -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */


__attribute__((__noinline__))
_Complex char
cc_regs_fun (_Complex char r0, _Complex char r1, 
	     _Complex char r2, _Complex char r3,
	     _Complex char r4, _Complex char r5,
	     _Complex char r6, _Complex char r7);

/*
**call_cc_regs_fun:
**	...
**	ldrh	w7, \[x0\]
**	ldrh	w6, \[x0, 2\]
**	ldrh	w5, \[x0, 4\]
**	ldrh	w4, \[x0, 6\]
**	ldrh	w3, \[x0, 8\]
**	ldrh	w2, \[x0, 10\]
**	ldrh	w1, \[x0, 12\]
**	ldrh	w0, \[x0, 14]\
**	bl	_cc_regs_fun
**	...
*/

_Complex char
call_cc_regs_fun (void)
{
  return cc_regs_fun ((_Complex char) (1 + 1i), (_Complex char) (2 + 2i),
		      (_Complex char) (3 + 3i), (_Complex char) (4 + 4i),
		      (_Complex char) (5 + 5i), (_Complex char) (6 + 6i),
		      (_Complex char) (7 + 7i), (_Complex char) (8 + 8i));
}


__attribute__((__noinline__))
_Complex short
cs_regs_fun (_Complex short r0, _Complex short r1,
	     _Complex short r2, _Complex short r3,
	     _Complex short r4, _Complex short r5,
	     _Complex short r6, _Complex short r7);

/*
**call_cs_regs_fun:
**	...
**	ldr	w7, \[x0, 16\]
**	ldr	w6, \[x0, 20\]
**	ldr	w5, \[x0, 24\]
**	ldr	w4, \[x0, 28\]
**	ldr	w3, \[x0, 32\]
**	ldr	w2, \[x0, 36\]
**	ldr	w1, \[x0, 40\]
**	ldr	w0, \[x0, 44\]
**	bl	_cs_regs_fun
**	...
*/

__attribute__((__noinline__))
_Complex short
call_cs_regs_fun (void)
{
  return cs_regs_fun ((_Complex short) (1 + 1i), (_Complex short) (2 + 2i),
		      (_Complex short) (3 + 3i), (_Complex short) (4 + 4i),
		      (_Complex short) (5 + 5i), (_Complex short) (6 + 6i),
		      (_Complex short) (7 + 7i), (_Complex short) (8 + 8i));
}

__attribute__((__noinline__))
_Complex int
ci_regs_fun (_Complex int r0, _Complex int r1,
	     _Complex int r2, _Complex int r3,
	     _Complex int r4, _Complex int r5,
	     _Complex int r6, _Complex int r7);

/*
**call_ci_regs_fun:
**	...
**	ldr	x7, \[x0, 48\]
**	ldr	x6, \[x0, 56\]
**	ldr	x5, \[x0, 64\]
**	ldr	x4, \[x0, 72\]
**	ldr	x3, \[x0, 80\]
**	ldr	x2, \[x0, 88\]
**	ldr	x1, \[x0, 96\]
**	ldr	x0, \[x0, 104\]
**	bl	_ci_regs_fun
**	...
*/

__attribute__((__noinline__))
_Complex int
call_ci_regs_fun (void)
{
  return ci_regs_fun ((_Complex int) (1 + 1i), (_Complex int) (2 + 2i),
		      (_Complex int) (3 + 3i), (_Complex int) (4 + 4i),
		      (_Complex int) (5 + 5i), (_Complex int) (6 + 6i),
		      (_Complex int) (7 + 7i), (_Complex int) (8 + 8i));
}
