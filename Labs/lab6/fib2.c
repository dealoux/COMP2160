#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef SMALL
  #define CACHE_SIZE 20
#else
  #define CACHE_SIZE 60
#endif

long cache[CACHE_SIZE];

long fib(int n)
{
  long result;
  
  if(n < CACHE_SIZE){
    if(n > 2)
      cache[n] = fib(n-1) + fib(n-2);
    result = cache[n];
  }
  else
    result = fib(n-1) + fib(n-2);

  return result;
}

int main( int argc, char *argv[] )
{
  // we really should check the input...
  int fibNum = atoi(argv[1]);
  cache[0] = 0;
  cache[1] = 1;
  cache[2] = 1;
  
  printf("The %d Fibonacci number is %ld\n", fibNum, fib(fibNum));
  
  return EXIT_SUCCESS;
}
