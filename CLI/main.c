//作者:邓智豪
#include "../Algorithm/index.h"
#include "./scanFormatInput.h"
#include "./formatOutput.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( void ){
  Result result;
  DescribeTable describeTable = scanFormatInput();

  int beginTime = clock();

  result = solveJSSP( describeTable );

  int endTime = clock();

  printResult( result );
  printf( "Time Used: %.3fs\n", ( endTime - beginTime ) / 1000.0 );
  printf( "End Time:%d\n", result.timeSpan );

  freeResult( result );

  return 0;
}
