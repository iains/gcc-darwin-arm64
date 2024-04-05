/* { dg-do compile } */
/* { dg-options "-O -fcommon -mno-pc-relative-literal-loads" } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* This checks that we perform the correct accesses for file-scope vars
   including GOT indirections.  */

double gd = 1.0;

__attribute__((__weak__))
double wd = 2.0;

__attribute__((__visibility__("hidden")))
double hd = 3.0;

__attribute__((__weak__, __visibility__("hidden")))
double whd = 4.0;

extern double ed;

double cd;

static double sd = 5.0;

struct {
  double a;
  double b;
} two_dbls = { 1.0, 42.0 };

double arr[3] = { 6.0, 7.0, 8.0 };

/*
**test:
**	adrp	x[0-9]+, _gd@PAGE
**	ldr	d[0-9]+, \[x[0-9]+, #_gd@PAGEOFF\]
**	adrp	x[0-9]+, lC0@PAGE
**	ldr	d[0-9]+, \[x[0-9]+, #lC0@PAGEOFF\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	adrp	x[0-9]+, _wd@GOTPAGE
**	ldr	x[0-9]+, \[x[0-9]+, _wd@GOTPAGEOFF\]
**	ldr	d[0-9]+, \[x[0-9]+\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	adrp	x[0-9]+, _hd@PAGE
**	ldr	d[0-9]+, \[x[0-9]+, #_hd@PAGEOFF\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	adrp	x[0-9]+, _whd@PAGE
**	ldr	d[0-9]+, \[x[0-9]+, #_whd@PAGEOFF\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	adrp	x[0-9]+, _ed@GOTPAGE
**	ldr	x[0-9]+, \[x[0-9]+, _ed@GOTPAGEOFF\]
**	ldr	d[0-9]+, \[x[0-9]+\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	adrp	x[0-9]+, _cd@GOTPAGE
**	ldr	x[0-9]+, \[x[0-9]+, _cd@GOTPAGEOFF\]
**	ldr	d[0-9]+, \[x[0-9]+\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	fmov	d[0-9]+, 5.0e\+0
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	adrp	x[0-9]+, _two_dbls@PAGE\+8
**	ldr	d[0-9]+, \[x[0-9]+, #_two_dbls@PAGEOFF\+8\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	adrp	x[0-9]+, _arr@PAGE\+16
**	ldr	d[0-9]+, \[x[0-9]+, #_arr@PAGEOFF\+16\]
**	fadd	d[0-9]+, d[0-9]+, d[0-9]+
**	ret
*/

double test (void)
{
  double x = 123456123456123456.0;
  x += gd;
  x += wd;
  x += hd;
  x += whd;
  x += ed;
  x += cd;
  x += sd;
  x += two_dbls.b;
  x += arr[2];

  return x;
}
