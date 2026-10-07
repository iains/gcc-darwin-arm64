/* PR target/124146 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23" } */
/* { dg-final { check-function-bodies "**" "" } } */

/* Like pr124146-3.c, but for _BitInt.  A _BitInt typedef that combines an
   alignment attribute with may_alias is its own TYPE_MAIN_VARIANT and keeps
   TYPE_USER_ALIGN set, which used to trip the assert in
   aarch64_function_arg_alignment (PR124146).  The AAPCS64 passes a _BitInt
   using the natural alignment of its container, ignoring user alignment: a
   _BitInt no wider than 64 bits occupies a single GPR, one between 65 and 128
   bits uses a 16-byte two-GPR container (so the C.8 rule rounds NGRN up to an
   even register), and one wider than 128 bits is passed by reference.  */

typedef __attribute__((__aligned__ (8), __may_alias__)) _BitInt(64) ma8_bi64;
typedef __attribute__((__aligned__, __may_alias__)) _BitInt(128) ma_bi128;
typedef __attribute__((__aligned__ (8), __may_alias__)) _BitInt(128) ma8_bi128;
typedef __attribute__((__aligned__, __may_alias__)) _BitInt(256) ma_bi256;

void consume_bi64 (int, _BitInt(64));
void consume_bi128 (int, _BitInt(128));
void consume_bi256 (int, _BitInt(256));

/* Over-aligned _BitInt(64): fits in a single 8-byte register, so alignment is
   irrelevant to placement -> argument in x1.
** pass_bi64:
**	mov	x1, x0
**	mov	w0, 5
**	b	_?consume_bi64
*/
void pass_bi64 (ma8_bi64 y) { consume_bi64 (5, y); }

/* Over-aligned _BitInt(128): natural alignment 16 -> argument in x2/x3.
** pass_bi128:
**	mov	x2, x0
**	mov	x3, x1
**	mov	w0, 5
**	b	_?consume_bi128
*/
void pass_bi128 (ma_bi128 y) { consume_bi128 (5, y); }

/* Under-aligned (aligned(8)) _BitInt(128): natural alignment is still 16, so
   the argument must still land in x2/x3, not x1/x2.
** pass_bi128_underaligned:
**	mov	x2, x0
**	mov	x3, x1
**	mov	w0, 5
**	b	_?consume_bi128
*/
void pass_bi128_underaligned (ma8_bi128 y) { consume_bi128 (5, y); }

/* A _BitInt wider than 128 bits is passed by reference, so the alignment code
   is not reached with the _BitInt type; just check it still compiles.  */
void pass_bi256 (ma_bi256 y) { consume_bi256 (5, y); }
