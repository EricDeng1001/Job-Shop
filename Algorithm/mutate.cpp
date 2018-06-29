//作者:黄云龙
#include "./mutate.h"
#include <stdlib.h>
#include <time.h>

Gene mutate( Gene origin ){
  ComponentIndex tmp;
  const int geneLength = origin.geneLength;
  int i = rand() % geneLength;
  int j = rand() % geneLength;

  // 随机选择两个位置交换
  tmp = origin.genes[j];
  origin.genes[j] = origin.genes[i];
  origin.genes[i] = tmp;

  return origin;
}
