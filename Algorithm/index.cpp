//作者:邓智豪
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define __populationSize__ 400
#define iterateNum 5000
#define __mutateRate__ 0.35

// 算法总模块
Result solveJSSP( DescribeTable describeTable ){
  Population population;
  Population newGeneration;
  Result result;
  Gene child1, child2;
  int randomPicker1, randomPicker2;
  int lastTimeSpan = 0;
  int sameResult = 0;
  double coin;

  // 生成基因
  population = newPopulation( __populationSize__, describeTable );

  // 解码一次, 计算出初始基因的适应度, 并且排序
  result = decodePopulation( population, describeTable );
  freeResult( result );

  newGeneration.populationSize = __populationSize__;
  newGeneration.individuals =
    (Gene*)malloc( sizeof( Gene ) * __populationSize__ );

  // 不断进化
  for( int i = 0; i < iterateNum; i++ ){

    for( int j = 0; j < __populationSize__ / 2; j++ ){
      randomPicker1 = rand() % __populationSize__ / 2;
      randomPicker2 = rand() % __populationSize__;

      // 选出两个基因交叉,产生两个新后代

      child1 = crossover(
        population.individuals[randomPicker1],
        population.individuals[randomPicker2],
        describeTable
      );

      child2 = crossover(
        population.individuals[randomPicker2],
        population.individuals[randomPicker1],
        describeTable
      );

      // 一定几率,后代变异
      coin = ( rand() % 100000 ) / 100000.0;
      if( coin >= __mutateRate__ ){
        mutate( child1 );
      }
      coin = ( rand() % 100000 ) / 100000.0;
      if( coin >= __mutateRate__ ){
        mutate( child2 );
      }

      newGeneration.individuals[2 * j] = child1;
      newGeneration.individuals[2 * j + 1] = child2;
    }

    // 计算适应度
    result = decodePopulation( newGeneration, describeTable );

    // 如果多次未出现进化, 重设随机种子
    if( lastTimeSpan == result.timeSpan ){
      sameResult++;
      if( sameResult >= ( describeTable.componentCount - 1 ) * describeTable.machineCount * 4 ){
        srand(time(NULL));
        sameResult = 0;
      }
    } else {
      lastTimeSpan = result.timeSpan;
      sameResult = 0;
    }

    freeResult( result );

    // 选择最优
    selectPopulation( population, newGeneration );

    for( int i = newGeneration.populationSize - 1; i >= 0; --i ){
      freeGene( newGeneration.individuals[i] );
    }
  }

  // 计算最优
  result = decodePopulation( population, describeTable );

  freePopulation( population );
  free( newGeneration.individuals );

  // 返回最优
  return result;
}
