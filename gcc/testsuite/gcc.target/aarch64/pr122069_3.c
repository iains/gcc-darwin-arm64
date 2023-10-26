/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8.2-a+dotprod -mautovec-preference=asimd-only --param vect-epilogues-nomask=0 -fno-schedule-insns -fno-reorder-blocks -fno-schedule-insns2 -fdump-tree-vect-details" }*/
/* { dg-final { check-function-bodies "**" "" } } */

/* Spelled unsigned so that this source means the same thing on every target.
   With a plain char parameter the 'i < 0' test is dead where char is unsigned
   (as on aarch64-linux) and live where it is signed (as on Darwin), so the two
   compile to different loops.  Unsigned is what the expected bodies below
   describe -- note it makes char_abs the identity, so the loop sums byte
   differences rather than absolute differences.  */
inline unsigned char char_abs(unsigned char i) {
  return (i < 0 ? -i : i);
}

/*
** foo_int:
** 	...
** 	sub	v[0-9]+.16b, v[0-9]+.16b, v[0-9]+.16b
** 	udot	v[0-9]+.4s, v[0-9]+.16b, v[0-9]+.16b
** 	...
*/
int foo_int(unsigned char *x, unsigned char * restrict y) {
  int sum = 0;
  for (int i = 0; i < 8000; i++)
     sum += char_abs(x[i] - y[i]);
  return sum;
}

/*
** foo2_int:
** 	...
** 	add	v[0-9]+.8h, v[0-9]+.8h, v[0-9]+.8h
** 	uadalp	v[0-9]+.4s, v[0-9]+.8h
** 	...
*/
int foo2_int(unsigned short *x, unsigned short * restrict y) {
  int sum = 0;
  for (int i = 0; i < 8000; i++)
    {
      x[i] = x[i] + y[i];
      sum += x[i];
    }
  return sum;
}

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 2 "vect" } } */
