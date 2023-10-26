/* { dg-options "-mgeneral-regs-only" } */

#include <stdarg.h>

typedef int int32x2_t __attribute__ ((__vector_size__ ((8))));

int
test (int i, ...)
{
  va_list argp;
  va_start (argp, i);
  int32x2_t x = (int32x2_t) {0, 1};
/* On AAPCS64 a vector va_arg has to be read from the FP register save area,
   which cannot be done without FP, hence the error.  darwinpcs passes every
   variadic argument on the stack, so there is no save area to read and the
   value is fetched with general registers alone; the diagnostic does not
   apply.  */
  x += va_arg (argp, int32x2_t); /* { dg-error "'-mgeneral-regs-only' is incompatible with the use of vector types" "" { target { ! *-*-darwin* } } } */
  return x[0] + x[1];
}
