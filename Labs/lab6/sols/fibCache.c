#include <stdio.h>
#include <stdlib.h>

#ifndef CACHE_SIZE
#define CACHE_SIZE 10
#endif

static long cache[CACHE_SIZE] = {0};

long fib(int n)
{
  long result;
  // we're not going to save the first 3 values so let's shift things over
  int cacheIndex = n-3;
  
  if (n==0)
    result = 0;
  else if (n==1 || n==2)
    result = 1;
  else if (cacheIndex<CACHE_SIZE && cache[cacheIndex])
    result = cache[cacheIndex];
  else
  {
    result = fib(n-1) + fib(n-2);
    if (cacheIndex<CACHE_SIZE)
      cache[cacheIndex] = result;
  }
  
  return result;
}

int main( int argc, char *argv[] )
{
  // we really should check the input...
  int fibNum = atoi(argv[1]);
  
  printf("The %d Fibonacci number is %ld\n", fibNum, fib(fibNum));
  
  return EXIT_SUCCESS;
}
