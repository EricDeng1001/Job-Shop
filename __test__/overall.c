//作者:谭典
#include "../Algorithm/index.h"
#include "../CLI/scanFormatInput.h"
#include "../CLI/formatOutput.h"
#include <stdio.h>
#include <stdlib.h>

#define __populationSize__ 100
#define iterateNum 2000
#define __mutateRate__ 0.15

int main( void ){
  Population population;
  Population newGeneration;
  Result result;
  Gene child1, child2;
  int randomPicker1, randomPicker2;
  double coin;
  DescribeTable describeTable = scanFormatInput();
  population = newPopulation( __populationSize__, describeTable );
  result = decodePopulation( population, describeTable );
  freeResult( result );

  newGeneration.populationSize = __populationSize__;
  newGeneration.individuals =
    (Gene*)malloc( sizeof( Gene ) * __populationSize__ );

  for( int i = 0; i < iterateNum; i++ ){

    for( int j = 0; j < __populationSize__ / 2; j++ ){
      randomPicker1 = rand() % __populationSize__ / 2;
      randomPicker2 = rand() % __populationSize__;

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

      coin = ( rand() % 100000 ) / 100000.0;
      if( coin > __mutateRate__ ){
        mutate( child1 );
      }
      coin = ( rand() % 100000 ) / 100000.0;
      if( coin > __mutateRate__ ){
        mutate( child2 );
      }

      newGeneration.individuals[2 * j] = child1;
      newGeneration.individuals[2 * j + 1] = child2;
    }

    result = decodePopulation( newGeneration, describeTable );

    printf("makeSpan:%d\n", result.timeSpan );

    freeResult( result );

    selectPopulation( population, newGeneration );

    for( int i = newGeneration.populationSize - 1; i >= 0; --i ){
      freeGene( newGeneration.individuals[i] );
    }
  }

  result = decodePopulation( population, describeTable );

  printResult( result );
  printf("makeSpan:%d\n", result.timeSpan );

  freePopulation( population );
  free( newGeneration.individuals );
  freeDescribeTable( describeTable );

  return 0;
};
