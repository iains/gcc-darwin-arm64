/* { dg-do compile } */
/* { dg-options "-fsanitize=shadow-call-stack -fno-exceptions" } */

int i;

/* aarch64-*-vxworks has x18 as a fixed register.  */
/* { dg-error "'-fsanitize=shadow-call-stack' requires '-ffixed-x18'" "" { target { aarch64*-*-* && { ! { aarch64-*-vxworks* || aarch64*-*-darwin* } } } } 0 } */
/* { dg-message "sorry, unimplemented: '-fsanitize=shadow-call-stack' conflicts with the use of register x18" "" { target { aarch64-*-vxworks* } } 0 } */
/* darwinpcs reserves x18 for the OS, so ShadowCallStack is not available at
   all; the generic diagnostic in toplev.cc reports that.  */
/* { dg-message "sorry, unimplemented: '-fsanitize=shadow-call-stack' not supported in current platform" "" { target aarch64*-*-darwin* } 0 } */
