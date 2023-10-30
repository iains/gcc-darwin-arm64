/* { dg-do compile } */
/* { dg-options "-O2 -fdump-rtl-expand-details" } */
/* { dg-final { check-function-bodies "**" "" } } */
/* PR middle-end/90693 */

#pragma GCC target "+nocssc"

/*
** le16:
*E	sub	w([0-9]+), w0, #1
*E	and	w([0-9]+), (?:w0, w\1|w\1, w0)
*E	tst	w\2, 65535
*M	sub	w([0-9]+), w0, #1
*M	tst	(?:w\1, w0|w0, w\1)
**	cset	w0, eq
**	ret
*/

unsigned le16 (const unsigned short a) {
  return __builtin_popcountg (a) <= 1;
}

/*
** gt16:
*E	sub	w([0-9]+), w0, #1
*E	and	w([0-9]+), (?:w0, w\1|w\1, w0)
*E	tst	w\2, 65535
*M	sub	w([0-9]+), w0, #1
*M	tst	(?:w\1, w0|w0, w\1)
**	cset	w0, ne
**	ret
*/
unsigned gt16 (const unsigned short a) {
  return __builtin_popcountg (a) > 1;
}
