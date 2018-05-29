#ifndef __GENE__H__
#define __GENE__H__
#include "./decribeTable.h"

typedef struct {
  ComponentIndex* genes;
  int geneLength;
  int timeSpan;
} Gene;//一个基因就是一串数字

typedef struct {
  Gene* individuals;
  int populationSize;
} Population; //基因组称为种群

Population newPopulation( int populationSize, DescribeTable table );

void freeGene( Gene gene );

void selectPopulation( Population p1, Population p2 );

void freePopulation( Population population );

void printPopulation( Population population );

void printGene( Gene gene );


#endif
