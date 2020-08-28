/* { dg-do compile } */
/* { dg-additional-options "-O " } */
/* { dg-final { check-function-bodies "**" "" "" { target *-*-darwin* } } } */

/* This will fail, because of issue #74
**foo: 
**	cmp	w0, w1
**	cset	w0, eq
**	ret
*/

__attribute__((__noinline__))
int
foo (char a, unsigned char b)
{
  return a == b ? 1 : 0;
}

__attribute__((__noinline__))
int
bar (short a, unsigned short b)
{
  return a == b ? 1 : 0;
}

void pop (char *, unsigned char *, short *, unsigned short *);

int main ()
{
   char a;
   unsigned char b;
   short c;
   unsigned short d;
   int result;
   pop (&a, &b, &c, &d);
   
   result = foo (a, b);
   result += bar (c, d);
   return result;
}
