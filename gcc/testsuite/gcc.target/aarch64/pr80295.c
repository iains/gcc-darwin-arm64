/* { dg-do compile } */
/* { dg-require-effective-target arm_mabi_ilp32 } */
/* { dg-options "-mabi=ilp32" } */

void f (void *b) 
{ 
  __builtin_update_setjmp_buf (b); 
}

