/* { dg-do run } */
/* { dg-skip-if "incompatible asm" { *-*-darwin* } } */
/* { dg-require-effective-target fstack_protector } */
/* { dg-require-effective-target fpic } */
/* { dg-options "-fstack-protector-all -O2 -fpic" } */

#include "stack-protector-1.c"
