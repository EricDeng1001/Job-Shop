//作者:邓智豪
#include "crossover.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printOrderList( OrderList l ){
  OrderListNode* tmp = l.head->next;

  while( tmp ){
    printf("%d,%d->", tmp->gene, tmp->order );
    tmp = tmp->next;
  }
  printf("\n");
}

OrderListNode* newNode( ComponentIndex gene, int order ){
  OrderListNode* ret = (OrderListNode*)malloc( sizeof( OrderListNode ) );
  ret->order = order;
  ret->gene = gene;
  ret->next = NULL;
  return ret;
}

OrderList createOrderList(){
  OrderList ret;
  ret.head = newNode( -1, -1 );
  ret.tail = ret.head;
  return ret;
}

void freeOrderList( OrderList l ){
  OrderListNode* tmp = l.head;
  OrderListNode* toFree;
  while( tmp ){
    toFree = tmp;
    tmp = tmp->next;
    free( toFree );
  }
}

void push( OrderList* l, ComponentIndex gene, int order ){
  OrderListNode* node = newNode( gene, order );
  l->tail->next = node;
  l->tail = node;
}

// 切分基因组, 准备插入
OrderList slice( OrderList origin, int begin, int end ){
  OrderList result = createOrderList();
  int i;
  OrderListNode* tmp = origin.head->next;
  for( i = 0; i < begin; i++ ){
    tmp = tmp->next;
  }
  for( i = begin; i < end; i++ ){
    push( &result, tmp->gene, tmp->order );
    tmp = tmp->next;
  }
  return result;
}

// 链接基因组
OrderList concat( OrderList left, OrderList middle, OrderList right ){
  middle.tail->next = right.head->next;
  left.tail->next = middle.head->next;
  free( middle.head );
  free( right.head );
  return left;
}

// 转换为基因
Gene toGene( OrderList l, int geneLength ){
  Gene result;
  OrderListNode* tmp = l.head->next;
  result.genes =
    (ComponentIndex*)malloc( sizeof( ComponentIndex ) * geneLength );
  for( int i = 0; i < geneLength; i++ ){
    result.genes[i] = tmp->gene;
    tmp = tmp->next;
  }
  result.geneLength = geneLength;
  return result;
}

// 去重
void findAndRemove( OrderList* l, OrderListNode* toCompare ){
  OrderListNode* tmp = l->head;
  OrderListNode* toFree;
  while( tmp ){
    if( tmp->next ){
      if( tmp->next->order == toCompare->order ){
        if( tmp->next->gene == toCompare->gene ){
          toFree = tmp->next;
          if( toFree->next == NULL ){ //删除的是尾节点
            l->tail = tmp;
          }
          tmp->next = tmp->next->next;
          free( toFree );
          return;
        }
      }
      tmp = tmp->next;
    } else {
      break;
    }
  }
}

// 对外暴露的函数,交叉两个基因
Gene crossover( Gene p1, Gene p2, DescribeTable table ){
  const int geneLength = p1.geneLength;
  const int componentCount = table.componentCount;
  int* componentCountTable = (int*)malloc( sizeof( int ) * componentCount );
  OrderList l1 = createOrderList();
  OrderList l2 = createOrderList();
  OrderList fragment;
  OrderList leftChild;
  OrderList rightChild;
  Gene result;

  int fragmentBegin = rand() % ( geneLength - 1 );
  int fragmentEnd = fragmentBegin + 1 + rand() % ( geneLength - fragmentBegin );
  int insertPort = rand() % geneLength;

  // 先把基因转换为有序链表,便于计算
  memset( componentCountTable, 0, sizeof( int ) * componentCount );
  for( int i = 0; i < geneLength; i++ ){
    // ( gene, order )
    push( &l1, p1.genes[i], componentCountTable[p1.genes[i]] );
    componentCountTable[p1.genes[i]]++;
  }
  memset( componentCountTable, 0, sizeof( int ) * componentCount );
  for( int i = 0; i < geneLength; i++ ){
    push( &l2, p2.genes[i], componentCountTable[p2.genes[i]] );
    componentCountTable[p2.genes[i]]++;
  }

  // 切割两个基因
  fragment = slice( l1, fragmentBegin, fragmentEnd );
  leftChild = slice( l2, 0, insertPort );
  rightChild = slice( l2, insertPort, geneLength );

  OrderListNode* tmp = fragment.head->next;
  for( int i = 0; i < fragmentEnd - fragmentBegin; i++ ){
    findAndRemove( &leftChild, tmp );
    findAndRemove( &rightChild, tmp );
    tmp = tmp->next;
  }

  // 链接,然后直接转回基因形式
  result = toGene( concat( leftChild, fragment, rightChild ), geneLength );

  freeOrderList( l1 );
  freeOrderList( l2 );
  freeOrderList( leftChild );
  free( componentCountTable );

  return result;
}
