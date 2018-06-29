//作者:邓智豪
#ifndef __FORMAT__OUTPUT__
#define __FORMAT__OUTPUT__
#include "../Algorithm/decode.h"
#include "repair.h"

typedef struct {
  int startTime;
  int componentIndex;
  int jobIndex;
  int endTime;
} Formater;

int endTimeCompare( Formater a, Formater b );

void sortByEndTime( Formater* formater, int size );

void printResult( Result result );

void printResultWithRepair( Result Result );
#endif
