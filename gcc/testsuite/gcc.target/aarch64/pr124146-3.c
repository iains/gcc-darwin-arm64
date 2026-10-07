/* PR target/124146 */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" } } */

/* The AAPCS64 passes scalars and vectors using their natural alignment,
   ignoring user alignment.  A 16-byte __int128 must therefore use its
   natural 16-byte alignment (so the C.8 rule rounds NGRN up to an even
   register), even when a may_alias typedef records a different user
   alignment.  These checks would have ICEd before the PR124146 fix.  */

typedef __attribute__((__aligned__, __may_alias__)) __int128 ma_i128;
typedef __attribute__((__aligned__ (8), __may_alias__)) __int128 ma8_i128;
typedef __attribute__((__aligned__, __may_alias__)) unsigned long ma_ul;

void consume_i128 (int, __int128);
void consume_ul (int, unsigned long);

/* Over-aligned __int128: natural alignment 16 -> argument in x2/x3.
** pass_i128:
**	mov	x2, x0
**	mov	x3, x1
**	mov	w0, 5
**	b	_?consume_i128
*/
void pass_i128 (ma_i128 y) { consume_i128 (5, y); }

/* Under-aligned (aligned(8)) __int128: natural alignment is still 16, so the
   argument must still land in x2/x3, not x1/x2.
** pass_i128_underaligned:
**	mov	x2, x0
**	mov	x3, x1
**	mov	w0, 5
**	b	_?consume_i128
*/
void pass_i128_underaligned (ma8_i128 y) { consume_i128 (5, y); }

/* Over-aligned unsigned long: a single 8-byte register, alignment is
   irrelevant to placement -> argument in x1.
** pass_ul:
**	mov	x1, x0
**	mov	w0, 5
**	b	_?consume_ul
*/
void pass_ul (ma_ul y) { consume_ul (5, y); }
