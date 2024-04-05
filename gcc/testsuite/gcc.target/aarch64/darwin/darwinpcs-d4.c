/* { dg-do compile } */
/* we need this for __int128.  */
/* { dg-options "-std=gnu99 " } */
/* { dg-additional-options "-O -fno-schedule-insns -fno-schedule-insns2 " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* we should use x0, x1 and x2 - not skip x1.
**foo:
**	eor	x0, x0, x1
**	orr	x0, x0, x2
**	cmp	x0, 0
**	cset	w0, eq
**	ret
*/

__attribute__((__noinline__))
int
foo (unsigned long long x,unsigned __int128 y)
{
  return x == y ? 1 : 0;
}

/* we should use x0, x1 and x2.
**bar:
**	eor	x2, x2, x0
**	orr	x2, x2, x1
**	cmp	x2, 0
**	cset	w0, eq
**	ret
*/

__attribute__((__noinline__))
int
bar (unsigned __int128 y, unsigned long long x)
{
  return x == y ? 1 : 0;
}

int fooo (unsigned long long x, unsigned __int128 y);
int baro (unsigned __int128 y, unsigned long long x);
int zooo (long long x, __int128 y);

/* We pass 64b in both calls, but loading the w regs
   is equivalent from zero-fill, if we pass negative values we must
   use the full width.
**main:
**	...
**	mov	w1, 25
**	mov	w2, 0
**	mov	w0, 10
**	bl	_fooo
**	mov	w2, 10
**	mov	w0, 25
**	mov	w1, 0
**	bl	_baro
**	mov	x1, -25
**	mov	x2, -1
**	mov	x0, -10
**	bl	_zooo
**	...
*/

int main ()
{
  unsigned long long x = 10;
  unsigned __int128 y = 25;
  int r = fooo (x, y);
  r += baro (y, x);
  x = -10;
  y = -25;
  r += zooo (x, y);
}
