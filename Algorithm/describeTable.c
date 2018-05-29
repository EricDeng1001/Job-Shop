#include "./decribeTable.h"
#include <stdio.h>
#include <stdlib.h>

void freeDescribeTable( DescribeTable table ){
  for( int i = 0; i < table.componentCount; i++ ){
    free( table.components[i].jobs );
  }
  free( table.components );
}

void printDescribeTable( DescribeTable table ){
  int machineCount = table.machineCount;
  int componentCount = table.componentCount;
  Component* components = table.components;
  int jobCount;
  printf( "machines:%d\n", machineCount );
  printf( "componnets:\n" );
  for( int i = 0; i < componentCount; i++ ){
    printf( "component %d\n", i );
    jobCount = components[i].jobCount;
    for( int j = 0; j < jobCount; j++ ){
      printf("requiredMachine:%d ", components[i].jobs[j].requiredMachine );
      printf("timeSpan:%d ", components[i].jobs[j].timeSpan );
    }
    printf("\n");
  }
}
