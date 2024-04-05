/* { dg-do compile } */
/* { dg-options "-std=gnu11" } */

/* darwinpcs D.9 (clang; not in the written spec): unnamed
   bit-fields, zero-width or not, do not contribute to the alignment of the
   containing structure.  A zero-width bit-field still moves the next field
   to a boundary of its declared type.  Named bit-fields do contribute.
   The expected values are those given by Apple clang for arm64-apple-darwin
   (clang for aarch64-linux gives the AAPCS64 values, e.g. 16/8 for Z1).  */

#include <stddef.h>

#define CHECK(T, SZ, AL) \
  _Static_assert (sizeof (struct T) == SZ, #T " size"); \
  _Static_assert (_Alignof (struct T) == AL, #T " alignment")
#define AT(T, F, OFF) \
  _Static_assert (offsetof (struct T, F) == OFF, #T "." #F " offset")

/* Zero-width unnamed bit-fields.  */
struct Z1 { char x; long long :0; char y; };
struct Z2 { char x; int :0; char y; };
struct Z3 { char x; short :0; char y; };
struct Z4 { char a; long long :0; };
struct Z5 { long long :0; char a; };

CHECK (Z1, 9, 1);  AT (Z1, y, 8);
CHECK (Z2, 5, 1);  AT (Z2, y, 4);
CHECK (Z3, 3, 1);  AT (Z3, y, 2);
CHECK (Z4, 8, 1);
CHECK (Z5, 1, 1);  AT (Z5, a, 0);

/* Non-zero-width unnamed bit-fields.  */
struct U1 { char x; int :5; char y; };
struct U2 { char x; long long :3; char y; };

CHECK (U1, 3, 1);  AT (U1, y, 2);
CHECK (U2, 3, 1);  AT (U2, y, 2);

/* A named bit-field does contribute (control).  */
struct M1 { char x; int b:5; char y; };

CHECK (M1, 4, 4);  AT (M1, y, 2);

/* Reduced from gcc.dg/compat/struct-layout-1 type 2570; the named members
   give the alignment here, the unnamed one only moves 'i'.  */
enum E3 { e3_m127 = -127, e3_0 = 0, e3_127 = 127 };
struct A { short a; unsigned int b[0]; enum E3 c; unsigned d:9; char e:8;
	   unsigned f:9; char g:5; long long :0; enum E3 i; };

CHECK (A, 20, 4);  AT (A, i, 16);
