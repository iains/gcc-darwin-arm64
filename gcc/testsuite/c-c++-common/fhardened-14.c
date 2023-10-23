/* { dg-do compile { target pie } } */
/* { dg-options "-fhardened -O -fno-PIE" } */

#ifdef __PIE__
# error "PIE enabled when it should not be"
#endif
