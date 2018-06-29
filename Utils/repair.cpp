//作者:邓智豪 黄云龙
#include "repair.h"
#include <stdlib.h>
#include <string.h>
#define max( a, b ) ( ( a ) > ( b ) ? ( a ) : ( b ) )

Result decodeWithRepair( Gene* gene, DescribeTable table ){
    RepairNode *tmp;
    const int componentCount = table.componentCount;
    const int machineCount = table.machineCount;
    const int geneLength = gene->geneLength;
    const Component* components = table.components;
    int* machines; //存放机器的空闲时间
    int* componentJobs; //存放工件的下一个工序
    Result result;

    result.machineCount = machineCount;
    result.componentCount = componentCount;
    result.timeSpan = 0;
    result.schedule = (Block**)malloc( sizeof( Block* ) * componentCount );
    for( int i = 0; i < componentCount; i++ ){
      result.schedule[i] = (Block*)malloc( sizeof( Block ) * machineCount );
      for( int j = 0; j < machineCount; j++ ){
        result.schedule[i][j].startTime = -1; // 意味着没有
      }
    }

    machines = (int*)malloc( sizeof( int ) * machineCount );
    memset( machines, 0, sizeof( int ) * machineCount );
    componentJobs = (int*)malloc( sizeof( int ) * componentCount );
    memset( componentJobs, 0, sizeof( int ) * componentCount );

    ComponentIndex componentIndex;
    int jobIndex;
    Job job;
    int lastIdleTime;

    for( int i = 0; i < geneLength; i++ ){
      componentIndex = gene->genes[i];
      jobIndex = componentJobs[componentIndex];
      job = components[componentIndex].jobs[jobIndex];
      lastIdleTime = machines[job.requiredMachine];

      if( jobIndex == 0 ){
        result.schedule[componentIndex][0].startTime = lastIdleTime;
      } else {
        result.schedule[componentIndex][jobIndex].startTime =
          max(
            lastIdleTime,
            result.schedule[componentIndex][jobIndex - 1].endTime
          );
      }
      result.schedule[componentIndex][jobIndex].endTime =
        result.schedule[componentIndex][jobIndex].startTime + job.timeSpan;
      machines[job.requiredMachine] =
        result.schedule[componentIndex][jobIndex].endTime;

      // repair logic goes here
      int diff;
      tmp = globalRepairTable->head;
      while( tmp->next ){
        if( tmp->checked ){
          tmp = tmp->next;
          continue;
        }
        if( tmp->machineIndex == job.requiredMachine ){
          if( result.schedule[componentIndex][jobIndex].startTime < tmp->startTime ){
            if( result.schedule[componentIndex][jobIndex].endTime > tmp->startTime ){
              diff = tmp->endTime - tmp->startTime;
              tmp->startTime = result.schedule[componentIndex][jobIndex].endTime;
              tmp->endTime = result.schedule[componentIndex][jobIndex].endTime + diff;
              machines[job.requiredMachine] = tmp->endTime;
              tmp->checked = true;
            }
          } else if( result.schedule[componentIndex][jobIndex].endTime < tmp->startTime ){
            if( result.schedule[componentIndex][jobIndex].endTime > tmp->startTime ){
              result.schedule[componentIndex][jobIndex].startTime = tmp->endTime;
              result.schedule[componentIndex][jobIndex].endTime = tmp->endTime + job.timeSpan;
              machines[job.requiredMachine] = result.schedule[componentIndex][jobIndex].endTime;
              tmp->checked = true;
            }
          }
        }
        tmp = tmp->next;
      }

      // end


      result.schedule[componentIndex][jobIndex].machineIndex =
        job.requiredMachine;

      result.timeSpan = max(
        machines[job.requiredMachine],
        result.timeSpan
      );

      componentJobs[componentIndex]++;
    }

    gene->timeSpan = result.timeSpan;

    free( machines );
    free( componentJobs );

    return result;
}

void addRepair( int machineIndex, int startTime, int endTime ){
  RepairNode *tmp = (RepairNode*)malloc( sizeof( RepairNode ) );
  tmp->startTime = startTime;
  tmp->machineIndex = machineIndex;
  tmp->endTime = endTime;
  tmp->next = nullptr;
  if( globalRepairTable == nullptr ){
    globalRepairTable = (Repair*)malloc( sizeof( Repair ) );
    globalRepairTable->head = globalRepairTable->tail =
      (RepairNode*)malloc( sizeof( RepairNode ) );
  }
  globalRepairTable->tail->next = tmp;
  globalRepairTable->tail = tmp;
}
