/* { dg-do compile } */
/* { dg-options "-O1 -mbig-endian" } */
/* { dg-require-effective-target aarch64_big_endian_nocache } */

/* PR rtl-optimization/124078 */


int __attribute__((__vector_size__(8))) v;
void foo(int a, int b) { v = (int __attribute__((__vector_size__(8)))){a,b}; }
