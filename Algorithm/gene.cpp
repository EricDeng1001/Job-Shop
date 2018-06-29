//作者:邓智豪
#include "gene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <time.h>
Gene bestGene;

void freeGene( Gene gene ){
  free( gene.genes );
}

void freePopulation( Population population ){
  for( int i = population.populationSize - 1; i >= 0; --i ){
    free( population.individuals[i].genes );
  }
  free( population.individuals );
}

void printGene( Gene gene ){
  for( int i = 0; i < gene.geneLength; i++ ){
    printf("%d", gene.genes[i] );
  }
  printf("\n");
}

void printPopulation( Population population ){
  int populationSize = population.populationSize;

  for( int i = populationSize - 1; i >= 0; --i ){
    printGene( population.individuals[i] );
  }

}

// 根据问题描述生成指定大小的新种群
Population newPopulation( int populationSize, DescribeTable table ){
  srand( time( NULL ) );
  const int componentCount = table.componentCount;
  Population population;
  int geneLength = 0;
  int jobCount;
  int geneIndex;

  for( int i = 0; i < componentCount; i++ ){ //计算基因长度
    geneLength += table.components[i].jobCount;
  }

  population.individuals = (Gene*)malloc( sizeof( Gene ) * populationSize );
  population.populationSize = populationSize;

  for( int i = 0; i < populationSize; i++ ){ //遍历个体

    population.individuals[i].genes =
      (ComponentIndex*)malloc( sizeof( ComponentIndex ) * geneLength );
    population.individuals[i].geneLength = geneLength;

    memset( population.individuals[i].genes, -1, geneLength * sizeof( ComponentIndex ) );

    for( int j = 0; j < componentCount; j++ ){ //遍历组件

      jobCount = table.components[j].jobCount;

      for( int _ = 0; _ < jobCount; _++ ){ //遍历工序

          geneIndex = rand() % geneLength; //先随机生成

          while( population.individuals[i].genes[geneIndex] != -1 ){ //直到是空
            ++geneIndex; //否则循环遍历
            if( geneIndex >= geneLength ){
              geneIndex = 0;
            }
          }

          population.individuals[i].genes[geneIndex] = j; //填写基因
      }
    }
  }
  return population;
}

int sortGene( Gene a, Gene b ){
  return a.timeSpan < b.timeSpan;
}

void sortGenes( Gene* genes, int size ){
  std::sort(
    genes,
    genes + size,
    sortGene
  );
}

void selectPopulation( Population p1, Population p2 ){
  Gene* tmp =
    (Gene*)malloc( sizeof( Gene ) * ( p1.populationSize + p2.populationSize ) );
  //printPopulation( p1 );
  for( int i = 0; i < p1.populationSize; i++ ){
    tmp[i] = p1.individuals[i];
  }
  for( int i = 0; i < p2.populationSize; i++ ){
    tmp[i + p1.populationSize] = p2.individuals[i];
  }

  // 按照适应度排序
  sortGenes( tmp, p1.populationSize + p2.populationSize );

  // 选出最优的
  for( int i = 0; i < p1.populationSize; i++ ){
    p1.individuals[i] = tmp[i];
  }

  for( int i = 0; i < p2.populationSize; i++ ){
    p2.individuals[i] = tmp[i + p1.populationSize];
  }

  free( tmp );
}
