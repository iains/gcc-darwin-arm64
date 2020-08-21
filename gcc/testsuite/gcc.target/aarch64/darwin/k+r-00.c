/* { dg-do compile } */

/* { dg-options "-std=gnu99 " } */
/* { dg-additional-options "-O2 -fsection-anchors" } */


/* What we care about here is that we get int loads from sp, sp+4 and sp+8.
 * This code will change when we implement darwinpcs d.3 - since the
 * promotions will no longer be needed (although they are harmless).
**test_k_r00:
**	ldrsb	w[0-9]+, \[sp, 4\]
**	ldr	x[0-9]+, \[sp, 8\]
**	...
**	ldrsb	w[0-9]+, \[sp\]
**	...
*/

const char *
test_k_r00 (r0, r1, r2, r3, r4, r5, r6, r7, a, b, c)
     char r0, r1, r2, r3, r4, r5, r6, r7;
     char a;
     char b;
     const char *c;
{
  if (a > 10 && b < 100)
    return c;
  return (char *)0;
}
