/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include <stdint.h>

/*
** extend_8to32_by2:
*E	sbfiz	w0, w0, 1, 8
*M	lsl	w0, w0, 1
**	ret
*/
int32_t extend_8to32_by2  (int8_t x)  { return x * 2; }

/*
** extend_16to32_by2:
*E	sbfiz	w0, w0, 1, 16
*M	lsl	w0, w0, 1
**	ret
*/
int32_t extend_16to32_by2 (int16_t x) { return x * 2; }

/*
** extend_8to64_by2:
*E	sbfiz	x0, x0, 1, 8
*M	sbfiz	x0, x0, 1, 32
**	ret
*/
int64_t extend_8to64_by2  (int8_t x)  { return x * 2; }

/*
** extend_16to64_by2:
*E	sbfiz	x0, x0, 1, 16
*M	sbfiz	x0, x0, 1, 32
**	ret
*/
int64_t extend_16to64_by2 (int16_t x) { return x * 2; }

/*
** extend_32to64_by2:
**	sbfiz	x0, x0, 1, 32
**	ret
*/
int64_t extend_32to64_by2 (int32_t x) { return 2 * x; }

/*
** extend_64to128_by2:
**	mov	x1, x0
**	lsl	x0, x0, 1
**	sbfx	x1, x1, 62, 1
**	ret
*/
__int128_t extend_64to128_by2  (long long x)  { return x * 2; }
