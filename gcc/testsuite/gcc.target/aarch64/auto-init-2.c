/* Verify pattern initialization for integer and pointer type automatic variables.  */
/* { dg-do compile } */
/* { dg-options "-O -ftrivial-auto-var-init=pattern -fdump-rtl-expand" } */

#ifndef __cplusplus
# define bool _Bool
#endif

enum E {
  N1 = 0,
  N2,
  N3
};

extern void bar (unsigned char, short, int, enum E, long, long long, int *, bool);

void foo()
{
  unsigned char temp1;
  short temp2;
  int temp3;
  enum E temp4;
  long temp5;
  long long temp6;
  int *temp7;
  bool temp8;

  bar (temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8);
  return;
}

/* { dg-final { scan-rtl-dump-times "0xfe\\\]" 1 "expand" } } */
/* { dg-final { scan-rtl-dump-times "0xfffffffffffffefe" 1 "expand" } } */
/* { dg-final { scan-rtl-dump-times "0xfffffffffefefefe" 2 "expand" } } */
/* { dg-final { scan-rtl-dump-times "0xfefefefefefefefe" 3 "expand" } } */
