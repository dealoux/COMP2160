#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "table.h"

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

static int testsPassed;
static int testsFailed;
static int currentSize;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

void testEmpty();
void testAdd();
void testRemove();
void testLeaks();
void initSuite();
void cleanSuite();
void testFound( int size, int nums[] );
void testNotFound( int size, int nums[] );
void testSize();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------


int main( int argc, char *argv[] )
{
  initSuite();
  
  printf( "Initiating tests.\n\n" );
  testEmpty();
  testAdd();
  testRemove();
  testLeaks();
  
  cleanSuite();
  printf( "\nTests completed successfully.\n" );
  
  return EXIT_SUCCESS;
}

// The suite setup function.

void initSuite()
{
  testsPassed = 0;
  testsFailed = 0;
  currentSize = 0;
}

// Test boundary conditions on an empty table

void testEmpty()
{

  int emptyNums[1] = { 9000 };
  
  printf( "\nTesting an empty table:" );
  printf( "\n-----------------------\n" );
  
  testNotFound( 1, emptyNums );

  if ( removeItem( 9000 ) )
  {
    printf( "\nFAILURE: removed an item from the empty table.\n" );
    testsFailed++;
  }
  else
  {
    printf( "\nPassed: unable to remove an item from the empty table.\n" );
    testsPassed++;
  }
  testSize();
}

// Test adding elements to table

void testAdd()
{
#define ADD_TEST_SIZE 4
  int inNums[ADD_TEST_SIZE] = { 4, 3, 2, 1 };
  int notNums[1] = { -1 };
  int i;
  
  printf( "\nTesting insertions:" );
  printf( "\n-------------------\n" );
  for ( i=0; i<ADD_TEST_SIZE ; i++ )
  {
    if ( insertItem( inNums[i] ) )
    {
      currentSize++;
      testFound( 1, &inNums[i] );
    }
    else
    {
      printf( "FAILED: did not insert %d into the table.\n", inNums[i] );
      testsFailed++;
    }
  }
  
  testFound( ADD_TEST_SIZE, inNums );
  testNotFound( 1, notNums );
  testSize();
}

// Test removing elements from table

void testRemove()
{
#define REMOVE_TEST_SIZE 2
  int outNums[REMOVE_TEST_SIZE] = { 4, 1 };  // delete first and last in the table
  int newNums[REMOVE_TEST_SIZE] = { 3, 2 };
  int i;
  
  printf( "\nTesting deletions:" );
  printf( "\n------------------\n" );
  for ( i=0 ; i<REMOVE_TEST_SIZE ; i++ )
  {
    if ( removeItem( outNums[i] ) )
    {
        currentSize--;
        testNotFound( 1, &outNums[i] );
    }
    else
    {
        printf( "FAILED: did not remove %d from the table.\n", outNums[i] );
        testsFailed++;
    }
  }
  
  testFound( REMOVE_TEST_SIZE, newNums );
  testNotFound( REMOVE_TEST_SIZE, outNums );
  testSize();
  clearTable();
  currentSize = 0;
}

// do a bunch of inserts and deletes to see if there's a leak

void testLeaks()
{
  int i;
#define LEAK_SIZE 1000
  int leakNums[1] = { LEAK_SIZE - ( LEAK_SIZE / 2 ) };

  printf( "\nTesting leaks:" );
  printf( "\n--------------\n" );
  
  for ( i=0 ; i<LEAK_SIZE ; i++ )
  {
    insertItem( i );
    currentSize++;
  }
  
  testFound( 1, leakNums );
  testSize();
  
  for ( i=0 ; i<LEAK_SIZE ; i++ )
  {
    removeItem( i );
    currentSize--;
  }
  
  testNotFound( 1, leakNums );
  testSize();
  
  // we should add a pause so we can check the memory footprint...
}

// The suite cleanup function.

void cleanSuite()
{
  clearTable();
  currentSize = 0;
  testSize();
  
  printf( "\nTest results:\n" );
  printf( "%d tests were run.\n", testsPassed+testsFailed );
  printf( "%d tests passed.\n", testsPassed );
  printf( "%d tests failed.\n", testsFailed );
}

// Ensure that a set of elements is in the table

void testFound( int size, int nums[] )
{
  int searchNum;
  Boolean result;
  int i;
  
  printf( "\nTesting search for elements that are in the table:" );
  printf( "\n--------------------------------------------------\n" );
  for ( i=0 ; i<size ; i++ )
  {
    searchNum = nums[i];
    result = search( searchNum );
    
    if (result)
    {
      printf( "Passed: \"%d\" was found\n", searchNum );
      testsPassed++;
    }
    else
    {
      printf( "FAILED: \"%d\" was not found\n", searchNum );
      testsFailed++;
    }
  }
}

// Ensure that a set of elements is not in the table

void testNotFound( int size, int nums[] )
{
  int searchNum;
  Boolean result;
  int i;
  
  printf( "\nTesting search for elements not in the table:" );
  printf( "\n---------------------------------------------\n" );

  for ( i=0 ; i<size ; i++ )
  {
    searchNum = nums[i];
    result = search( searchNum );
    
    if (result)
    {
      printf( "FAILED: \"%d\" was found\n", searchNum );
      testsFailed++;
    }
    else
    {
      printf( "Passed: \"%d\" was not found\n", searchNum );
      testsPassed++;
    }
  }
}

// Print the elements in the table

void testSize()
{
  int num;
  Boolean result;
  int size;
  
  printf( "\nTesting size of table:" );
  printf( "\n----------------------\n" );
  result = firstItem( &num );
  size = 0;
  while ( result )
  {
    //printf("%s\n", string);
    size++;
    result = nextItem( &num );
  }
  printf( "Size of table is %d\n", size );
  
  if ( size == currentSize )
  {
    printf( "Passed: size of table is \"%d\", expected \"%d\"\n", size, currentSize );
    testsPassed++;
  }
  else
  {
    printf( "FAILED: size of table is \"%d\", expected \"%d\"\n", size, currentSize );
    testsFailed++;
  }
}
