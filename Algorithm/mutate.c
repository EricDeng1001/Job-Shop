#include "./mutate.h"
#include <stdlib.h>
#include <time.h>

Gene mutate( Gene origin ){
  ComponentIndex tmp;
  const int geneLength = origin.geneLength;
  int i = rand() % geneLength;
  int j = rand() % geneLength;

  tmp = origin.genes[j];
  origin.genes[j] = origin.genes[i];
  origin.genes[i] = tmp;

  return origin;
}
