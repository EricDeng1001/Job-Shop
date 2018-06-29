//作者:邓智豪
#ifndef DECODE__H__
#define DECODE__H__
#include "gene.h"

typedef struct {
  int startTime;
  int endTime;
  MachineIndex machineIndex;
} Block;

typedef struct {
  Block** schedule;
  int timeSpan;
  int machineCount;
  int componentCount;
} Result;

Result decodeGene( Gene* gene, DescribeTable table );
// 返回最优
Result decodePopulation( Population population, DescribeTable table );

void freeResult( Result result );

#endif
