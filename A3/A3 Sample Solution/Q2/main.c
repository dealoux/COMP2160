#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "set.h"

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------
#define MAX_SIZE 32

#define NOT_SIZE 1

typedef struct
{
  int expected[MAX_SIZE];
  int size;
  
  char *testName;
  
  // only needed for tests where we take 2 sets as input to an operation
  int inA[MAX_SIZE];
  int inASize;
  int inB[MAX_SIZE];
  int inBSize;
} TestCase;

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

static int testsPassed;
static int testsFailed;

// used to verify correctly indicating inequality across our set modification tests (insert/remove/empty) 
static int notItems[NOT_SIZE] = {42};

static int emptyItems[0] = {};


//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

void testEmpty();
void testInsert();
void testRemove();

void testUnion();
void testDisjoint();
void testDifference();

void testLeaks();
void initSuite();
void endSuite();

void testEqual( Set const * const theSet, const int size, const int items[] );
void testNotEqual( Set const * const theSet, const int size, const int items[] );
void testMemory();
Set *buildSet( const int size, const int items[] );

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------


int main( int argc, char *argv[] )
{
  initSuite();
  
  printf( "Initiating tests...\n" );

  // test insertItem first so I know that my buildSet will work as expected in subsequent tests
  // also used, along with testEmpty, to ensure that equal is correct
  testInsert();

  testEmpty();
  testRemove();
  
  testDisjoint();
  testUnion();
  testDifference();

  testLeaks();
  
  endSuite();
  printf( "\nTests completed successfully.\n" );
  
  return EXIT_SUCCESS;
}

void initSuite()
{
  testsPassed = 0;
  testsFailed = 0;
}

void endSuite()
{
  printf( "\nTest results:\n" );
  printf( "%d tests were run.\n", testsPassed+testsFailed );
  printf( "%d tests passed.\n", testsPassed );
  printf( "%d tests failed.\n", testsFailed );
}

void testEmpty()
{
  Set *theSet = newSet();
  
  assert(theSet);
  if (theSet)
  {
    printf( "\nTesting an empty set:\n" );
    printf( "---------------------\n" );
    testEqual( theSet, 0, emptyItems );
    testNotEqual( theSet, NOT_SIZE, notItems );
    if (removeItem( theSet, 1 ))
    {
      printf( "\nFailure: removed an item for an empty set.\n" );
      testsFailed++;
    }
    else
    {
      printf( "\nSuccess: unable to remove an item from an empty set.\n" );
      testsPassed++;
    }
    
    deleteSet(theSet);
    testMemory();
  }
}

void testInsert()
{
#define INSERT_TESTS 5
  int inItems[INSERT_TESTS] = {5, 5, 2, 3, 2};
  TestCase expectedItems[INSERT_TESTS] = {{{5}, 1, "Insert 5 into an empty set"}, 
                                          {{5}, 1, "Insert duplicate item 5 into a 1 item set"},
                                          {{5, 2}, 2, "Insert 2 into a 1 item set"}, 
                                          {{5, 2, 3}, 3, "Insert 3 into an n item set"}, 
                                          {{5, 2, 3}, 3, "Insert duplicate item 2 into an n item set"}
                                         };
  Boolean expectedReturn[INSERT_TESTS] = {true, false, true, true, false};

  Set *theSet = newSet();
  
  assert(theSet);
  if (theSet)
  {
    printf( "\nTesting insertions:\n" );
    printf( "-------------------\n" );
    for (int i=0; i<INSERT_TESTS; i++)
    {
      printf("\n%s:", expectedItems[i].testName);
  
      if (insertItem(theSet, inItems[i]) == expectedReturn[i])
      {
        testEqual( theSet, expectedItems[i].size, expectedItems[i].expected );
        testNotEqual( theSet, NOT_SIZE, notItems );
        testNotEqual( theSet, 0, emptyItems );
      }
      else
      {
        if (expectedReturn[i])
          printf( "\nFailure: expected %d to not be in the set.\n", inItems[i] );
        else
          printf( "\nFailure: expected %d to be in the set.\n", inItems[i] );
        
        testsFailed++;
      }
    }
    
    deleteSet(theSet);
    testMemory();
  }
}

void testDisjoint()
{
#define DISJOINT_TESTS 8
  Set *setA;
  Set *setB;
  // note that we don't use the expected resulting set here
  TestCase expectedItems[DISJOINT_TESTS] = {{{}, 0, "Two empty sets", {}, 0, {}, 0}, 
                                            {{}, 0, "First set empty", {}, 0, {1, 2, 3}, 3}, 
                                            {{}, 0, "Second set empty", {1, 2, 3}, 3, {}, 0}, 
                                            {{}, 0, "Identical 1 item sets", {1}, 1, {1}, 1}, 
                                            {{}, 0, "Identical n item sets", {1, 2, 3}, 3, {1, 2, 3}, 3}, 
                                            {{}, 0, "Unique 1 item sets", {1}, 1, {4}, 1}, 
                                            {{}, 0, "Unique n item sets", {1, 2, 3}, 3, {4, 5}, 2}, 
                                            {{}, 0, "Overlapping sets", {1, 2, 3, 4}, 4, {3, 4, 5, 6}, 4}
                                           };
  Boolean expectedReturn[DISJOINT_TESTS] = {true, true, true, false, false, true, true, false};

  printf( "\nTesting disjoint sets:\n" );
  printf( "----------------------\n" );
  for (int i=0; i<DISJOINT_TESTS; i++)
  {
    printf("\n%s:", expectedItems[i].testName);

    setA = buildSet(expectedItems[i].inASize, expectedItems[i].inA);
    setB = buildSet(expectedItems[i].inBSize, expectedItems[i].inB);

    assert(setA);
    assert(setB);
    if (setA && setB)
    {
      if (areDisjoint(setA, setB) == expectedReturn[i])
      {
        printf( " Success.\n" );
        testsPassed++;
      }
      else
      {
        if (expectedReturn[i])
          printf( " Failure: expected the sets to be disjoint.\n" );
        else
          printf( " Failure: expected the sets to not be disjoint.\n" );

        testsFailed++;
      }
  
      deleteSet(setA);
      deleteSet(setB);
    }
  }
  
  testMemory();
}

// NOTE: There is no inequality testing required for these three operations (remove, union, and difference)
//       due to the fact that they create a new set -- verifying that the new set is 
//       what we expect is sufficient. Testing for inequality would imply testing against *all* possible 
//       sets, which is untenable.

void testRemove()
{
#define REMOVE_TESTS 5
  int inItems[3] = {1, 3, 5};
  Set *theSet = buildSet(3, inItems);

  int outItems[REMOVE_TESTS] = {2, 1, 5, 7, 3};
  TestCase expectedItems[REMOVE_TESTS] = {{{1, 3, 5}, 3, "Don't remove 2 from an n item set"}, 
                                          {{3, 5}, 2, "Remove 1 from an n item set"}, 
                                          {{3}, 1, "Remove 5 to create a 1 item set"}, 
                                          {{3}, 1, "Don't remove 7 from a 1 item set"}, 
                                          {{}, 0, "Remove 3 to create an empty set"}
                                         };
  Boolean expectedReturn[REMOVE_TESTS] = {false, true, true, false, true};

  assert(theSet);
  if (theSet)
  {    
    printf( "\nTesting removes:\n" );
    printf( "----------------\n" );
    for (int i=0; i<REMOVE_TESTS; i++)
    {
      printf("\n%s:", expectedItems[i].testName);
  
      if (removeItem(theSet, outItems[i]) == expectedReturn[i])
      {
        testEqual( theSet, expectedItems[i].size, expectedItems[i].expected );
      }
      else
      {
        if (expectedReturn[i])
          printf( "\nFailure: expected %d to be in the set.\n", outItems[i] );
        else
          printf( "\nFailure: expected %d to not be in the set.\n", outItems[i] );
        
        testsFailed++;
      }
    }
  
    deleteSet(theSet);
    testMemory();
  }
}

void testUnion()
{
#define UNION_TESTS 8
  Set *setA;
  Set *setB;
  Set *unionSet;
  TestCase expectedItems[UNION_TESTS] = {{{}, 0, "Two empty sets", {}, 0, {}, 0}, 
                                         {{1, 2, 3}, 3, "First set empty", {}, 0, {1, 2, 3}, 3}, 
                                         {{1, 2, 3}, 3, "Second set empty", {1, 2, 3}, 3, {}, 0}, 
                                         {{1}, 1, "Identical 1 item sets", {1}, 1, {1}, 1}, 
                                         {{1, 2, 3}, 3, "Identical n item sets", {1, 2, 3}, 3, {1, 2, 3}, 3}, 
                                         {{1, 4}, 2, "Unique 1 item sets", {1}, 1, {4}, 1}, 
                                         {{1, 2, 3, 4, 5}, 5, "Unique n item sets", {1, 2, 3}, 3, {4, 5}, 2}, 
                                         {{1, 2, 3, 4, 5, 6}, 6, "Overlapping sets", {1, 2, 3, 4}, 4, {3, 4, 5, 6}, 4}
                                        };

  printf( "\nTesting unions:\n" );
  printf( "---------------\n" );
  for (int i=0; i<UNION_TESTS; i++)
  {
    printf("\n%s:", expectedItems[i].testName);
    setA = buildSet(expectedItems[i].inASize, expectedItems[i].inA);
    setB = buildSet(expectedItems[i].inBSize, expectedItems[i].inB);

    assert(setA);
    assert(setB);
    if (setA && setB)
    {
      unionSet = unionOf(setA, setB);

      assert(unionSet);
      if (unionSet)
      {
        testEqual(unionSet, expectedItems[i].size, expectedItems[i].expected);
        deleteSet(unionSet);
      }
      
      deleteSet(setA);
      deleteSet(setB);
    }
  }
  
  testMemory();
}

void testDifference()
{
#define DIFF_TESTS 8
  Set *setA;
  Set *setB;
  Set *diffSet;
  TestCase expectedItems[DIFF_TESTS] = {{{}, 0, "Two empty sets", {}, 0, {}, 0}, 
                                        {{1, 2, 3}, 3, "First set empty", {}, 0, {1, 2, 3}, 3}, 
                                        {{1, 2, 3}, 3, "Second set empty", {1, 2, 3}, 3, {}, 0}, 
                                        {{}, 0, "Identical 1 item sets", {1}, 1, {1}, 1}, 
                                        {{}, 0, "Identical n item sets", {1, 2, 3}, 3, {1, 2, 3}, 3}, 
                                        {{1, 4}, 2, "Unique 1 item sets", {1}, 1, {4}, 1}, 
                                        {{1, 2, 3, 4, 5}, 5, "Unique n item sets", {1, 2, 3}, 3, {4, 5}, 2}, 
                                        {{1, 3, 6}, 3, "Overlapping sets", {1, 2, 3, 4}, 4, {2, 4, 6}, 3}
                                       };

  printf( "\nTesting differences:\n" );
  printf( "--------------------\n" );
  for (int i=0; i<DIFF_TESTS; i++)
  {
    printf("\n%s:", expectedItems[i].testName);
    setA = buildSet(expectedItems[i].inASize, expectedItems[i].inA);
    setB = buildSet(expectedItems[i].inBSize, expectedItems[i].inB);

    assert(setA);
    assert(setB);
    if (setA && setB)
    {
      diffSet = symmetricDifferenceOf(setA, setB);

      assert(diffSet);
      if (diffSet)
      {
        testEqual(diffSet, expectedItems[i].size, expectedItems[i].expected);
        deleteSet(diffSet);
      }
      
      deleteSet(setA);
      deleteSet(setB);
    }
  }
  
  testMemory();
}

void testLeaks()
{
#define NUM_SETS 1000000
  Set *mySets[NUM_SETS];
  int inItems[3] = {2, 4, 6};

  printf( "\nTesting for leaks:\n" );
  printf( "------------------\n" );
  
  for (int i=0; i<NUM_SETS; i++ )
  {
    mySets[i] = buildSet(3, inItems);
  }
  
  for (int i=0; i<NUM_SETS; i++ )
  {
    deleteSet(mySets[i]);
  }
  
  testMemory();
  
  // we should add a pause so we can check the memory footprint...
#ifdef USE_PAUSE
  getchar();
#endif
}

//
// These routines provide support for testing the other set operations.
//

// Ensure that a set of elements is in theSet
void testEqual( Set const * const theSet, const int size, const int items[] )
{
  Set *compareSet = buildSet(size, items);
  
  assert(compareSet);
  if (compareSet)
  {
    printf("\nTesting equality for items that are in the set...\n");
    if (areEqual(theSet, compareSet))
    {
      printf( "Success: the set contains exactly the expected items.\n" );
      testsPassed++;
    }
    else
    {
      printf( "Failure: the set does not contain all, and only all, the expected items.\n" );
      testsFailed++;
    }
  
    deleteSet(compareSet);
  }
}

// Ensure that we correctly detect when sets are different
void testNotEqual( Set const * const theSet, const int size, const int items[] )
{
  Set *compareSet = buildSet(size, items);
  
  assert(compareSet);
  if (compareSet)
  {
    printf("\nTesting inequality of items in the set...\n");
    if (!areEqual(theSet, compareSet))
    {
      printf( "Success: the set does not contain the expected items.\n" );
      testsPassed++;
    }
    else
    {
      printf( "Failure: the set contains the expected items.\n" );
      testsFailed++;
    }
  
    deleteSet(compareSet);
  }
}

// Utility routine: create a set with the given elements
Set *buildSet( const int size, const int items[] )
{
  Set *theSet = newSet();
  
  if (theSet)
  {
    for (int i=0; i<size; i++)
      insertItem(theSet, items[i]);
  }
    
  return theSet;
}

void testMemory()
{
  printf( "\nTesting memory footprint:\n" );
  if (validateMemUse())
  {
    printf( "Success: no memory leaks detected.\n" );
    testsPassed++;
  }
  else
  {
    printf( "Failure: extra Set objects detected.\n" );
    testsFailed++;
  }
}
