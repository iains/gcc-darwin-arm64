/* { dg-do compile } */
/* { dg-options "-O2 -march=armv9.5-a" } */
/* { dg-final { check-function-bodies "**" "" "" } } */

#define vect16 __attribute__((vector_size(16)))

vect16 char fff(char _292, char _145, char _231)
{
    return (vect16 char) {_292, _145, _145, _231, _292, _145, _145, _231, _292, _145, _145, _231, _292, _145, _145, _231};
}

/* On aarch64-darwin, darwinpcs D.3 promotes the narrow arguments in the caller,
   so the callee sees subreg:QI (reg:SI) and @aarch64_simd_vec_set<mode> takes
   its FP alternative -- dup into a vector register, then ins -- where the
   expected body collects the bytes in a general register with bfi.  That is one
   instruction more, and the same cause as the vec-init-23 cases: see the
   comment there for the control that reproduces the choice on ELF.  Longer
   rather than merely different, so the body is xfailed rather than given a *M
   variant; it will XPASS if the cost comparison is improved.  */

/*
** fff: { xfail *-*-darwin* }
**	dup	v31\.8b, w1
**	fmov	w1, s31
**	bfi	w1, w0, 0, 8
**	bfi	w1, w2, 24, 8
**	dup	v0\.4s, w1
**	ret
*/
