/* { dg-options "-O2" } */

void sc_callee () [[arm::streaming_compatible]];
void s_callee () [[arm::streaming]];
void n_callee ();

[[arm::locally_streaming]] __attribute__((noipa)) void
sc_ls_callee () [[arm::streaming_compatible]] {}
[[arm::locally_streaming]] __attribute__((noipa)) void
n_ls_callee () {}

void
n_to_sc ()
{
  sc_callee ();
}
/* { dg-final { scan-assembler {\tb\t_?sc_callee} } } */

void
n_to_s ()
{
  s_callee ();
}
/* { dg-final { scan-assembler {\tbl\t_?s_callee} } } */

void
n_to_n ()
{
  n_callee ();
}
/* { dg-final { scan-assembler {\tb\t_?n_callee} } } */

void
n_to_sc_ls ()
{
  sc_ls_callee ();
}
/* { dg-final { scan-assembler {\tb\t_?sc_ls_callee} } } */

void
n_to_n_ls ()
{
  n_ls_callee ();
}
/* { dg-final { scan-assembler {\tb\t_?n_ls_callee} } } */
