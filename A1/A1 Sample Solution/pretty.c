#include <stdio.h>
#include <string.h>

#define  LINE_SIZE  50

// assume we never read more than 80 characters on a line
#define MAX_LINE    80
  
int main( int argc, char* argv[] )
{
  char inputLine[MAX_LINE];
  int position = 1;
  int length;
  
  // we need to check for NULL but the array is never NULL so we need to look at the return code...
  while ( fgets( inputLine, MAX_LINE, stdin ) )
  {
    // never make the same call more than once!
    // substract 1 to account for the EOL character -- since it's a Unix file there is only 1 (Windows has 2)
    length = strlen( inputLine ) - 1;
    // get rid of the EOL character so we don't have to worry about it anymore
    inputLine[length] = '\0';
    
    if ( length )
    {
      // if our line is long enough start a new one
      if ( (position + length - 1) > LINE_SIZE)
      {
        printf( "\n" );
        position = 1;
      }
      
      printf( "%*s", length, inputLine);
      position += length;
      
      // add a blank after the current word, unless we're going to skip to a new line on the next iteration
      if (position <= LINE_SIZE)
      {	
        printf( " " );
        position++;
      }
    }
    
    // we have a blank line so include a blank line
    else
    {
      if ( position > 1 )
      {
        printf( "\n" );
      }
      
      printf( "\n" );
      position = 1;
    }
  }
  
  return 0;
}
