//作者:邓智豪
#ifndef __DESCRIBETABLE__H__
#define __DESCRIBETABLE__H__

typedef int ComponentIndex;
typedef int MachineIndex;

typedef struct {
  MachineIndex requiredMachine;
  int timeSpan;
} Job;

typedef struct {
  Job* jobs;
  int jobCount;
} Component;

typedef struct {
  Component* components;
  int machineCount;
  int componentCount;
} DescribeTable; //问题描述表

void freeDescribeTable( DescribeTable table );

void printDescribeTable( DescribeTable table );

#endif
