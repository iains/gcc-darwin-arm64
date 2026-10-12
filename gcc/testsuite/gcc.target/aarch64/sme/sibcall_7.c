/* { dg-options "-O2" } */

void shared_callee () [[arm::inout("za")]];
[[arm::new("za")]] __attribute__((noipa)) void new_callee () {}
void normal_callee ();

[[arm::new("za")]] void
new_to_shared ()
{
  shared_callee ();
}
/* { dg-final { scan-assembler {\tbl\t_?shared_callee} } } */

[[arm::new("za")]] void
new_to_new ()
{
  new_callee ();
}
/* { dg-final { scan-assembler {\tb\t_?new_callee} } } */

[[arm::new("za")]] void
new_to_normal ()
{
  normal_callee ();
}
/* { dg-final { scan-assembler {\tb\t_?normal_callee} } } */
