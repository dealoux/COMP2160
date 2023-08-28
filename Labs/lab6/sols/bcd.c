#include <stdio.h>
#include <stdlib.h>

// the number of bits for each BCD value
#define BCD_OFFSET 4

// generates a BCD word
unsigned short word2bcd( short value )
{
  unsigned short next;
  unsigned short bcd = 0;
  unsigned short multiplier = BCD_OFFSET;

  bcd = value % 10;
  value /= 10;
  // string off values but make sure we don't go past our return size -- could make int...
  while ( value && (multiplier <= (BCD_OFFSET*sizeof(unsigned short))) )
  {
    next = value % 10;
    value /= 10;

    bcd += (next<<multiplier);
    multiplier += BCD_OFFSET;
  }
  
  return bcd;
}

// converts the passed date to its BCD equivalent
unsigned long bcdDate( int day, int month, int year )
{
  unsigned long date;

  // NOTE: given how this works, you can use += instead of |= to get the same results

  date = word2bcd( day );
  date <<= (BCD_OFFSET*2);
  date |= word2bcd( month );
  date <<= (BCD_OFFSET*2);
  date |= word2bcd( year/100 );
  date <<= (BCD_OFFSET*2);
  date |= word2bcd( year%100 );
  
  return date;
}

int main( int argc, char *argv[] )
{
  int day;
  int month;
  int year;
  unsigned long date;

  while ( EOF != scanf( "%d-%d-%d", &day, &month, &year ) )
  {
    date = bcdDate( day, month, year );
    printf( "%d-%d-%d %08lx %ld\n", day, month, year, date, date );
  }
  
  return EXIT_SUCCESS;
}
