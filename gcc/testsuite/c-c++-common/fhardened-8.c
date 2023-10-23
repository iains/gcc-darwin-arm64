/* { dg-do compile { target pie } } */
/* { dg-options "-fhardened -O -fPIC" } */

/* -fPIC takes precedence over -fhardened */
#ifdef __PIE__
# error "PIE enabled when it should not be"
#endif
