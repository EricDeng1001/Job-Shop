#include "./decode.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define max( a, b ) ( ( a ) > ( b ) ? ( a ) : ( b ) )

Result decodeGene( Gene* gene, DescribeTable table ){
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
    result.schedule[componentIndex][jobIndex].machineIndex =
      job.requiredMachine;

    machines[job.requiredMachine] =
      result.schedule[componentIndex][jobIndex].endTime;
    result.timeSpan = max( machines[job.requiredMachine], result.timeSpan );

    componentJobs[componentIndex]++;
  }

  gene->timeSpan = result.timeSpan;

  free( machines );
  free( componentJobs );

  return result;
}

Result decodePopulation( Population population, DescribeTable table ){
  Result tmp;
  Result best = decodeGene( &population.individuals[0], table );

  for( int i = 1; i < population.populationSize; i++ ){
    tmp = decodeGene( &population.individuals[i], table );
    if( tmp.timeSpan < best.timeSpan ){
      freeResult( best );
      best = tmp;
    } else {
      freeResult( tmp );
    }
  }

  return best;
}

void freeResult( Result result ){
  for( int i = 0; i < result.componentCount; i++ ){
    free( result.schedule[i] );
  }
  free( result.schedule );
}
