/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define N 8
short a[N] = {0};
short b[N] = {0};

/*
** foo:
**	...
*E	ldr	q[0-9]+, \[x[0-9]+, #:lo12:\.LANCHOR0\]
*E	ldr	q[0-9]+, \[x[0-9]+, 16\]
*M	ldr	q[0-9]+, \[x[0-9]+, #_a@PAGEOFF\]
*M	ldr	q[0-9]+, \[x[0-9]+, #_b@PAGEOFF\]
**	cmgt	v[0-9]+\.8h, v[0-9]+\.8h, v[0-9]+\.8h
**	umaxp	v[0-9]+\.4s, v[0-9]+\.4s, v[0-9]+\.4s
**	fmov	x[0-9]+, d[0-9]+
**	cmp	x[0-9]+, 0
**	cset	w0, ne
**	ret
*/
__attribute__ ((noipa, noinline))
short foo (void)
{
  for (int i = 0; i < N; i++)
    {
      if (a[i] > b[i])
	return 1;
    }
  return 0;
}
