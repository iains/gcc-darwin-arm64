/* { dg-do assemble { target { aarch64*-*-* } } } */
/* { dg-require-effective-target aarch64_asm_bf16_ok }  */
/* { dg-require-effective-target arm_v8_2a_bf16_neon_ok } */
/* { dg-add-options arm_v8_2a_bf16_neon } */
/* { dg-additional-options "-save-temps" } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" { target { ! aarch64*-*-darwin* } } } } */

#include <arm_neon.h>


/*
**test_bfmmla:
**     bfmmla	v0.4s, v1.8h, v2.8h
**     ret
*/
float32x4_t test_bfmmla (float32x4_t r, bfloat16x8_t x, bfloat16x8_t y)
{
  return vbfmmlaq_f32 (r, x, y);
}
