#include "./scanFormatInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DescribeTable scanFormatInput(){
  DescribeTable result;
  int componentCount;
  int machineCount;
  int j;

  scanf( "%d %d\n", &result.componentCount, &result.machineCount );

  componentCount = result.componentCount;
  machineCount = result.machineCount;

  result.components =
    (Component*)malloc( sizeof( Component ) * componentCount );

  for( int i = 0; i < componentCount; i++ ){
    result.components[i].jobs =
      (Job*)malloc( sizeof( Job ) * machineCount );
    memset( result.components[i].jobs, 0, machineCount * sizeof( Job ) );

    j = 0;
    while( 1 ){
      scanf( "%d %d",
        &result.components[i].jobs[j].requiredMachine,
        &result.components[i].jobs[j].timeSpan
      );
      j++;
      if( getchar() == '\n' ){
        break;
      }
    }
    result.components[i].jobCount = j;
  }

  return result;
}
