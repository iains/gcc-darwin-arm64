/* { dg-do compile } */
/* { dg-options "-mrvv-vector-bits=scalable -march=rv32gcv -mabi=ilp32 -fno-schedule-insns -fno-schedule-insns2" } */

#include "riscv_vector.h"

void foo(void *in1, void *in2, void *in3, void *out, size_t n, size_t vl) {
  for (size_t i = 0; i < n; i += 1) {
    vint32m1_t a = __riscv_vle32_v_i32m1(in1, __riscv_vsetvl_e16m1(vl));
    vint32m1_t b = __riscv_vle32_v_i32m1_tu(a, in2, __riscv_vsetvl_e16m1(vl));
    vint32m1_t c = __riscv_vle32_v_i32m1_tu(b, in3, __riscv_vsetvl_e16m1(vl));
    __riscv_vse32_v_i32m1(out, c, __riscv_vsetvl_e16m1(vl));
    
    vint8mf2_t a2 = __riscv_vle8_v_i8mf2(in1 + 100, __riscv_vsetvl_e8m1(vl));
    vint8mf2_t b2 = __riscv_vle8_v_i8mf2_tu(a2, in2 + 100, __riscv_vsetvl_e8m1(vl));
    vint8mf2_t c2 = __riscv_vle8_v_i8mf2_tu(b2, in3 + 100, __riscv_vsetvl_e8m1(vl));
    __riscv_vse8_v_i8mf2(out + 100, c2, __riscv_vsetvl_e8m1(vl));
  }
}

/* { dg-final { scan-assembler-times {vsetvli} 4 { target { no-opts "-O0" no-opts "-g" no-opts "-funroll-loops" } } } } */
