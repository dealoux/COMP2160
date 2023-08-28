#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//-------------------------------------------------------------------------------------
// CONSTANTS
//-------------------------------------------------------------------------------------

#define MAX_LINE 32

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------
int testsExecuted = 0;
int testsFailed = 0;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

void testDistance( char *s, char *t, int expected );
int levenshtein( char *s, char *t );
int minimum( int list[], int length );
void copy( char src[], char dst[], int start );

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------


int main( int argc, char *argv[] )
{
  printf( "Testing typical cases.\n" );
  testDistance( "kitten", "kitten", 0 );
  testDistance( "kitten", "sitting", 3 );
  testDistance( "kitten", "mittens", 2 );
  testDistance( "balloon", "saloon", 2 );
  testDistance( "hello", "doggo", 4 );
  testDistance( "\t\thi", "\t\t\t\thi", 2 );

  printf( "\nTesting empty/edge cases.\n" );
  testDistance( "", "", 0 );
  testDistance( "hello", "", 5 );
  testDistance( "", "doggo", 5 );

  printf( "\nThis might take a while...\n" );
  testDistance( "12345678901", "123456789012", 1 );   // how many chars are we looking at?
  
  printf( "\nThese tests will not succeed in the C version\n" );
  testDistance( "kitten", "mitten\0s", 3 );           // ????
  testDistance( "\0totally", "\0different", 9 );

  printf( "\nTotal number of tests executed: %d\n", testsExecuted );
  printf( "Number of tests passed: %d\n", (testsExecuted - testsFailed) );
  printf( "Number of tests failed: %d\n", testsFailed );
  
  printf( "\nEnd of processing" );
  
  return( EXIT_SUCCESS );
  
}

void testDistance( char *s, char *t, int expected )
{
  int distance = levenshtein( s, t );

  if ( distance == expected )
  {
    printf( "Passed! You can get from '%s' to '%s' in %d moves.\n", s, t, expected );
  }
  else
  {
    printf( "FAILED: I thought it would take %d moves, but apparently it takes %d moves to get from '%s' to '%s'.\n", expected, distance, s, t );
    testsFailed++;
  }

  testsExecuted++;
}

int levenshtein( char *s, char *t )
{
  int cost;
  int distance;
  char deleteS[MAX_LINE];
  char deleteT[MAX_LINE];

  assert( s != NULL );
  assert( t != NULL );

#define BRANCHES 3
  int list[BRANCHES];

  if ( strlen( s ) == 0 )
  {
    distance = strlen( t );
  }
  else if ( strlen( t ) == 0 )
  {
    distance = strlen( s );
  }
  else
  {
    if ( s[0] == t[0] )
    {
      cost = 0;
    }
    else
    {
      cost = 1;
    }

    // is this really the best way? (hint: we're dropping the first char
    // and we're passing a pointer to a char array..., do we need to
    // copy *anything*?)
    // strcpy( deleteS, &s[1] );
    // strcpy( deleteT, &t[1] );
    copy( s, deleteS, 1 );
    copy( t, deleteT, 1 );

    assert( strlen( deleteS ) == strlen( s ) - 1 );
    assert( strlen( deleteT ) == strlen( t ) - 1 );
    
    // maybe you wrote your own endswith?
    assert( strcmp( deleteS, &s[1] ) == 0 );
    assert( strcmp( deleteT, &t[1] ) == 0 );

    list[0] = levenshtein( deleteS, t ) + 1;
    list[1] = levenshtein( s, deleteT ) + 1;
    list[2] = levenshtein( deleteS, deleteT ) + cost;

    distance = minimum( list, BRANCHES );
  }

  return distance;
}

int minimum( int list[], int length )
{
  int min = 0;
  assert( length > 0 );

  if ( length > 0 )
  {
    min = list[0];
    for ( int i = 1; i < length; i++ )
    {
      if ( list[i] < min )
      {
        min = list[i];
      }
    }
  }

  return min;
}

void copy(char src[], char dst[], int start)
{
    int i = start;
    for( ; src[i] != '\0'; i++ )
    {
        dst[i - start] = src[i];
    }
    dst[i - start] = '\0';
}
