/* { dg-do assemble } */
/* { dg-options "-O2 -march=armv8.1-a -save-temps" } */

/* darwinpcs D.4: a 16-byte argument may start at an odd register (x1:x2,
   x3:x4, ...), but CASP needs even/odd register pairs.  The odd-pair
   arguments must be copied to even pairs before the CASP; the assembler
   rejects an odd first register.  */

__int128
g2 (long a, __int128 *p, long b, __int128 o, __int128 n)
{
  return __sync_val_compare_and_swap (p, o, n);
}

void
g3 (long a, __int128 o, long b, __int128 n, __int128 *p)
{
  __sync_bool_compare_and_swap (p, o, n);
}

void
g1 (long a, __int128 o, __int128 n, __int128 *p)
{
  __atomic_compare_exchange_n (p, &o, n, 0, 0, 0);
}

__int128
g4 (long a, __int128 v, __int128 *p)
{
  return __atomic_fetch_add (p, v, 5);
}

/* { dg-final { scan-assembler-times {\tcaspal\tx[0-9]*[02468], x[0-9]*[13579], x[0-9]*[02468], x[0-9]*[13579], } 2 } } */
