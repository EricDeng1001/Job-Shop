//作者:邓智豪
#include "formatOutput.h"
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>

struct RepairCursorNode;

struct RepairCursorNode {
  int cursor;
  int startTime;
  int endTime;
  RepairCursorNode *next;
};

typedef struct {
  RepairCursorNode *head;
  RepairCursorNode *tail;
} RepairCursor;

int endTimeCompare( Formater a, Formater b ){
  return a.endTime < b.endTime;
}

void sortByEndTime( Formater* formater, int size ){
  std::sort(
    formater,
    formater + size,
    endTimeCompare
  );
}

void printResultWithRepair( Result result ){
    RepairNode *tmp;
    RepairCursorNode *usedToRecord;
    RepairCursor *printRecord = (RepairCursor*)malloc( sizeof( RepairCursor ) );
    Formater* formater =
      (Formater*)malloc( sizeof( Formater ) * result.componentCount );//不会超过组件数
    int index;
    for( int i = 0; i < result.machineCount; i++ ){
      // record this mahcine's repair block
      printRecord->head = printRecord->tail = (RepairCursorNode*)malloc( sizeof( RepairCursorNode ) );
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

      tmp = globalRepairTable->head;
      while( tmp->next ){
          tmp->checked = false;
          tmp = tmp->next;
      }
      tmp = globalRepairTable->head;
      // this loop calc every repair block's index in their machine
      while( tmp->next ){
        if( tmp->machineIndex == i ){ // for this machine
          for( int h = 0; h < index; h++ ){ // modify the arrangement and insert repair block
            if( formater[h].startTime >= tmp->endTime ){
              if( !tmp->checked ){
                usedToRecord = (RepairCursorNode*)malloc( sizeof( RepairCursorNode ) );
                usedToRecord->cursor = h; // original h should be print to repair block
                usedToRecord->next = NULL;
                usedToRecord->startTime = tmp->startTime;
                usedToRecord->endTime = tmp->endTime;
                printRecord->tail->next = usedToRecord;
                printRecord->tail = usedToRecord;
                tmp->checked = true;
              }
            }
          }
        }
        tmp = tmp->next;
      }

      usedToRecord = printRecord->head->next; // point to first and get ready to itterate

      for( int i = 0; i < index; i++ ){
        if( usedToRecord != nullptr ){
          if( i == usedToRecord->cursor ){
            printf(" (%d,检修,%d)",
              usedToRecord->startTime,
              usedToRecord->endTime
            );
          }
          usedToRecord = usedToRecord->next;
        }
        printf(" (%d,%d-%d,%d)",
          formater[i].startTime,
          formater[i].componentIndex,
          formater[i].jobIndex,
          formater[i].endTime
        );
      }
      printf("\n");
      // TODO: free the printRecord;
    }

    free( formater );
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
