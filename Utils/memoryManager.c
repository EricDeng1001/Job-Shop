#include "memoryManager.h"

struct memoryCollectorNode;

typedef struct {
  memoryCollectorNode* next;
  void *ptr;
} memoryCollectorNode;

typedef struct {
  memoryCollectorNode *head;
  memoryCollectorNode *tail;
} memoryManager;

static memoryCollectorNode initHead = { NULL, NULL };
static memoryCollectorNode memoryManager = { initHead, initHead };

void* allocMemory( size_t size ){
  void* result = malloc( size );
  memoryCollectorNode *newOne =
    (memoryCollectorNode*)malloc( sizeof( memoryCollectorNode ) );
  newOne->next = NULL;
  newOne->ptr = result;
  memoryManager.tail->next = newOne;
  memoryManager.tail = newOne;
  return result;
}

void garbageCollect(){
  memoryCollectorNode *tmp = memoryManager.head;
  memoryCollectorNode *toFree;
  while( tmp ){
    toFree = tmp;
    tmp = tmp->next;
    free( toFree.ptr );
    free( toFree );
  }
}
