//作者:邓智豪
#include "formatOutput.h"
#include <stdio.h>
#include <stdlib.h>

int endTimeCompare( const void* a, const void* b ){
  return ((Formater*)a)->endTime - ((Formater*)b)->endTime;
}

void sortByEndTime( Formater* formater, int size ){
  qsort(
    formater,
    size,
    sizeof( Formater ),
    endTimeCompare
  );
}

void printResult( Result result ){
  Formater* formater =
    (Formater*)malloc( sizeof( Formater ) * result.componentCount );//不会超过组件数
  int index;
  for( int i = 0; i < result.machineCount; i++ ){
    index = 0;
    printf( "M%d", i );
    // 二重循环遍历安排表
    for( int j = 0; j < result.componentCount; j++ ){
      for( int k = 0; k < result.machineCount; k++ ){
        if( result.schedule[j][k].startTime == -1 ){ //第j个工件结束
          break;
        }
        if( result.schedule[j][k].machineIndex == i ){ //第j个工件的第k个工序
          formater[index].startTime = result.schedule[j][k].startTime;
          formater[index].componentIndex = j;
          formater[index].jobIndex = k;
          formater[index].endTime = result.schedule[j][k].endTime;
          index++;
        }
      }
    }

    sortByEndTime( formater, index );

    for( int i = 0; i < index; i++ ){
      printf(" (%d,%d-%d,%d)",
        formater[i].startTime,
        formater[i].componentIndex,
        formater[i].jobIndex,
        formater[i].endTime
      );
    }
    printf("\n");
  }

  free( formater );
}
