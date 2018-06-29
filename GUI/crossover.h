//作者:邓智豪
#ifndef __CROSSOVER__H__
#define __CROSSOVER__H__
#include "gene.h"

struct OrderListNode;
typedef struct OrderListNode OrderListNode;

struct OrderListNode {
  ComponentIndex gene;
  int order;
  OrderListNode* next;
};

typedef struct {
  OrderListNode* head;
  OrderListNode* tail;
} OrderList;

void printOrderList( OrderList l );

OrderListNode* newNode( ComponentIndex gene, int order );

OrderList createOrderList();

void freeOrderList( OrderList l );

void push( OrderList* l, ComponentIndex gene, int order );

OrderList slice( OrderList origin, int begin, int end );

OrderList concat( OrderList left, OrderList middle, OrderList right );

Gene toGene( OrderList l, int geneLength );

void findAndRemove( OrderList* l, OrderListNode* toCompare );

Gene crossover( Gene p1, Gene p2, DescribeTable table );

#endif
