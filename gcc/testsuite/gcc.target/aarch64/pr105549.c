/* { dg-do compile } */
/* { dg-skip-if "darwinpcs D.4: aligned x-reg passing differs" { *-*-darwin* } } */
/* { dg-options "-save-temps" } */

enum e { E1 };
typedef enum e e __attribute__((aligned(16)));
union u {
    __attribute__((aligned(2), packed)) e a : 1;
    int x[4];
};
union u g(int a, union u u2) { return u2; }

/* { dg-final { scan-assembler "stp\tx1, x2, \\\[sp, 8\\\]" } } */
