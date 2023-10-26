/* { dg-do compile } */
/* { dg-options "-fhardened -fstack-protector" } */

#ifdef __SSP_STRONG__
# error "-fstack-protector-strong enabled when it should not be"
#endif
#ifndef __SSP__
# error "-fstack-protector not enabled"
#endif

/* { dg-warning ".-fstack-protector-strong. is not enabled" "" { target *-*-* } 0 } */
/* { dg-warning "._FORTIFY_SOURCE. is not enabled" "" { target *-*-* } 0 } */
