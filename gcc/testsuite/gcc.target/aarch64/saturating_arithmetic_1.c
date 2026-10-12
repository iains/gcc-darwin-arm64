/* { dg-do compile } */
/* { dg-options "-O2 --save-temps -fno-schedule-insns2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

/*
** uadd:
*E	dup	v([0-9]+).8b, w[01]
*E	dup	v([0-9]+).8b, w[01]
*E	uqadd	b([0-9]+), (?:b\2, b\1|b\1, b\2)
*E	umov	w0, v\3.b\[0\]
*M	fmov	s([0-9]+), w0
*M	fmov	s([0-9]+), w1
*M	uqadd	b\1, b\1, b\2
*M	umov	w0, v\1\.b\[0\]
**	ret
*/
/*
** uadd2:
*E	dup	v([0-9]+).8b, w[01]
*E	dup	v([0-9]+).8b, w[01]
*E	uqadd	b([0-9]+), (?:b\2, b\1|b\1, b\2)
*E	umov	w0, v\3.b\[0\]
*M	fmov	s([0-9]+), w0
*M	fmov	s([0-9]+), w1
*M	uqadd	b\1, b\1, b\2
*M	umov	w0, v\1\.b\[0\]
**	ret
*/
/*
** usub:
*E	dup	v([0-9]+).8b, w[01]
*E	dup	v([0-9]+).8b, w[01]
*E	uqsub	b([0-9]+), b\1, b\2
*E	umov	w0, v\3.b\[0\]
*M	fmov	s([0-9]+), w0
*M	fmov	s([0-9]+), w1
*M	uqsub	b\1, b\1, b\2
*M	umov	w0, v\1\.b\[0\]
**	ret
*/

#include <limits.h>

#define UT unsigned char
#define UMAX UCHAR_MAX
#define UMIN 0

#include "saturating_arithmetic.inc"
