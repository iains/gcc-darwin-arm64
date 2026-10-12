/* { dg-do compile } */
/* { dg-options "-O2 --save-temps -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** uadd:
*E	dup	v([0-9]+).4h, w[01]
*E	dup	v([0-9]+).4h, w[01]
*E	uqadd	h([0-9]+), (?:h\2, h\1|h\1, h\2)
*E	umov	w0, v\3.h\[0\]
*M	fmov	s([0-9]+), w0
*M	fmov	s([0-9]+), w1
*M	uqadd	h\1, h\1, h\2
*M	umov	w0, v\1\.h\[0\]
**	ret
*/
/*
** uadd2:
*E	dup	v([0-9]+).4h, w[01]
*E	dup	v([0-9]+).4h, w[01]
*E	uqadd	h([0-9]+), (?:h\2, h\1|h\1, h\2)
*E	umov	w0, v\3.h\[0\]
*M	fmov	s([0-9]+), w0
*M	fmov	s([0-9]+), w1
*M	uqadd	h\1, h\1, h\2
*M	umov	w0, v\1\.h\[0\]
**	ret
*/
/*
** usub: { xfail *-*-* }
**	dup	v([0-9]+).4h, w1
**	dup	v([0-9]+).4h, w0
**	uqsub	h([0-9]+), h\1, h\2
**	umov	w0, v\3.h\[0\]
**	ret
*/

#include <limits.h>

#define UT unsigned short
#define UMAX USHRT_MAX
#define UMIN 0

#include "saturating_arithmetic.inc"
