/* { dg-do compile } */
/* { dg-options "-fhardened -O -fdump-tree-gimple" } */

int
foo ()
{
  int i;
  return i;
}

/* { dg-final { scan-tree-dump ".DEFERRED_INIT" "gimple" } } */
