/* The const-variable-array-size extension must not substitute a non-constant
   initializer: the array type would then be variably modified by an expression
   that needs a temporary, and the temporary is not remapped into the OpenMP
   child function, so the outlined body referred to a decl that does not belong
   to it.  Such a declaration has to stay an ordinary VLA.  */

/* { dg-do compile } */
/* { dg-additional-options "-O -fallow-const-var-array-size" } */

void
reproduce (const int ghosts, const double dx, const int nx, const int count,
	   const double *points)
{
  const int order = 2 * ghosts + 1;
  const double inverse_dx = 1.0 / dx;

#pragma omp parallel for
  for (int i = 0; i < count; ++i)
    {
      const int index = (int) (points[i] * inverse_dx + 0.5);
      if (index - ghosts < 0 || index + ghosts >= nx)
	{}
      double local[order];
      (void) local;
    }
}
