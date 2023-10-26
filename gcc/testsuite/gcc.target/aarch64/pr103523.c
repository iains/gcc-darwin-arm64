/* { dg-do compile } */
/* { dg-skip-if "SVE is not available on Darwin" { *-*-darwin* } } */
/* { dg-options "-march=armv8-a+sve -mtune=neoverse-v1 -Ofast" } */

void d(float *a, float b, int c) {
    float e;
    for (; c; c--, e += b)
      a[c] = e;
}
