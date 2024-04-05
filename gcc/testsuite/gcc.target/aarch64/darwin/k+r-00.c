/* { dg-do compile } */

/* { dg-options "-std=gnu99 " } */
/* { dg-additional-options "-O2" } */
/* { dg-final { check-function-bodies "**" "" "" } } */


/* A K&R definition: the char parameters a and b undergo the default argument
   promotions, so they are passed as int and occupy 4-byte stack slots at sp
   and sp+4; c follows at sp+8.
**test_k_r00:
**	ldrsb	w[0-9]+, \[sp, 4\]
**	ldrsb	w[0-9]+, \[sp\]
**	...
**	ldr	x[0-9]+, \[sp, 8\]
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
