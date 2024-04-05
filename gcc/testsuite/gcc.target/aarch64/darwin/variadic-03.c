/* { dg-do run } */
/* { dg-options "-std=gnu23 -O2" } */

/* darwinpcs packs named stack arguments with their natural alignment [D.2],
   but the first unnamed argument of a variadic call always starts at the next
   stack slot (and is aligned as for AAPCS64).  Check that caller and callee
   agree on where the unnamed arguments are for each sub-word position that
   the last named stack argument can end at.  The named arguments that fill
   the argument registers force the interesting ones onto the stack.  */

#include <stdarg.h>

#define L8 long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7
#define LV 1, 2, 3, 4, 5, 6, 7, 8
#define D8 double d0, double d1, double d2, double d3, double d4, double d5, \
	   double d6, double d7
#define DV 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0

#define VA 0x1234567890L, -42, 3.5

#define CHECK_VA						\
  do {								\
    va_list ap;							\
    va_start (ap);						\
    long vl = va_arg (ap, long);				\
    int vi = va_arg (ap, int);					\
    double vd = va_arg (ap, double);				\
    va_end (ap);						\
    if (vl != 0x1234567890L || vi != -42 || vd != 3.5)		\
      __builtin_abort ();					\
  } while (0)

#define CHECK(c) do { if (!(c)) __builtin_abort (); } while (0)

/* Last named stack argument ends at offset 1 .. 8 within its slot.  */

__attribute__((noipa)) void
f1 (L8, char p, ...)
{ CHECK (p == 11); CHECK_VA; }

__attribute__((noipa)) void
f2 (L8, char p, char q, ...)
{ CHECK (p == 11 && q == 12); CHECK_VA; }

__attribute__((noipa)) void
f3 (L8, char p, char q, char r, ...)
{ CHECK (p == 11 && q == 12 && r == 13); CHECK_VA; }

__attribute__((noipa)) void
f3s (L8, short p, char q, ...)
{ CHECK (p == 11 && q == 12); CHECK_VA; }

__attribute__((noipa)) void
f4 (L8, char p, short q, ...)
{ CHECK (p == 11 && q == 12); CHECK_VA; }

__attribute__((noipa)) void
f4i (L8, int p, ...)
{ CHECK (p == 11); CHECK_VA; }

__attribute__((noipa)) void
f5 (L8, int p, char q, ...)
{ CHECK (p == 11 && q == 12); CHECK_VA; }

__attribute__((noipa)) void
f6 (L8, short p, short q, short r, ...)
{ CHECK (p == 11 && q == 12 && r == 13); CHECK_VA; }

__attribute__((noipa)) void
f7 (L8, int p, short q, char r, ...)
{ CHECK (p == 11 && q == 12 && r == 13); CHECK_VA; }

__attribute__((noipa)) void
f8 (L8, char p, int q, ...)
{ CHECK (p == 11 && q == 12); CHECK_VA; }

/* The same with FP arguments, once the FP argument registers are full.  */

__attribute__((noipa)) void
g4 (D8, float p, ...)
{ CHECK (p == 11.0f); CHECK_VA; }

__attribute__((noipa)) void
g8 (D8, float p, float q, ...)
{ CHECK (p == 11.0f && q == 12.0f); CHECK_VA; }

__attribute__((noipa)) void
h5 (L8, D8, float p, char q, ...)
{ CHECK (p == 11.0f && q == 12); CHECK_VA; }

__attribute__((noipa)) void
h8 (L8, D8, char p, float q, ...)
{ CHECK (p == 11 && q == 12.0f); CHECK_VA; }

/* The last named argument is in a register, but earlier ones are packed on
   the stack.  */

__attribute__((noipa)) void
r1 (L8, char p, double q, ...)
{ CHECK (p == 11 && q == 12.0); CHECK_VA; }

__attribute__((noipa)) void
r6 (L8, short p, int q, float r, ...)
{ CHECK (p == 11 && q == 12 && r == 13.0f); CHECK_VA; }

/* An unnamed argument that needs 16-byte alignment.  */

__attribute__((noipa)) void
q2 (L8, char p, char q, ...)
{
  CHECK (p == 11 && q == 12);
  va_list ap;
  va_start (ap);
  __int128 v = va_arg (ap, __int128);
  long l = va_arg (ap, long);
  va_end (ap);
  CHECK (v == (((__int128) 0x1122334455667788L << 64) | 0x99aabbccddeeff00UL));
  CHECK (l == 0x1234567890L);
}

int
main (void)
{
  f1 (LV, 11, VA);
  f2 (LV, 11, 12, VA);
  f3 (LV, 11, 12, 13, VA);
  f3s (LV, 11, 12, VA);
  f4 (LV, 11, 12, VA);
  f4i (LV, 11, VA);
  f5 (LV, 11, 12, VA);
  f6 (LV, 11, 12, 13, VA);
  f7 (LV, 11, 12, 13, VA);
  f8 (LV, 11, 12, VA);
  g4 (DV, 11.0f, VA);
  g8 (DV, 11.0f, 12.0f, VA);
  h5 (LV, DV, 11.0f, 12, VA);
  h8 (LV, DV, 11, 12.0f, VA);
  r1 (LV, 11, 12.0, VA);
  r6 (LV, 11, 12, 13.0f, VA);
  q2 (LV, 11, 12,
      ((__int128) 0x1122334455667788L << 64) | 0x99aabbccddeeff00UL,
      0x1234567890L);
  return 0;
}
