#ifndef __FORMAT__OUTPUT__
#define __FORMAT__OUTPUT__
#include "../Algorithm/decode.h"

typedef struct {
  int startTime;
  int componentIndex;
  int jobIndex;
  int endTime;
} Formater;

void printResult( Result result );

#endif
