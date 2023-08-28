//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 1, QUESTION: question 2
// 
// REMARKS: This program calculates the steps between a few different strings using leveshtein algorithm.
//
//-----------------------------------------

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX 3
// define true false for readability
#define TRUE 1
#define FALSE 0

int endsWith(char[], char[]);
void subString(char[], char[], int, int);
void testDistance(char[], char[], int);
int levenshtein(char[], char[]);
int minimum(int[]);

int testsFailed = 0;
int testsExecuted = 0;

int main(void) {
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
    testDistance( "a", "b", 1 );
    testDistance( "b", "b", 0 );
    testDistance( " ", " ", 0 );

    printf( "\nThis might take a while...\n" );
    testDistance( "12345678901", "123456789012", 1 );   // how many chars are we looking at?

    printf( "\n******These tests will be opposite in the C version******\n" );
    printf( "\n******These tests **should** FAIL in the C version*******\n" );
    testDistance( "kitten", "mitten\0s", 3 );           // ????
    testDistance( "\0totally", "\0different", 9 );

    printf( "\nTotal number of tests executed: %u\n", testsExecuted );
    printf( "Number of tests passed: %u\n", testsExecuted - testsFailed );
    printf( "Number of tests failed: %u\n", testsFailed );

    return 0;
}


/** Helper methods */
// This method returns TRUE (1) if the given char array (string) ends with the given value or FALSE (0) otherwise
int endsWith(char string[], char value[]) {
    int result = TRUE;

    if (strlen(value) <= strlen(string)) {
        int i = strlen(string) - 1;

        for (int j = strlen(value) - 1; j >= 0; j--) {
            if (string[i] != value[j]) {
                result = FALSE;
            }
            i--;
        }
    }
    else {
        result = FALSE;
    }

    return result;
} // end of endsWith(...) helper method

// This method returns the sub-string of the given string from index start to and include index end
void subString(char string[], char result[], int start, int end) {
    int i = 0;
    if (start <= end && end < strlen(string)) {
        for (int j = start; j <= end; j++) {
            result[i] = string[j];
            i++;
        }

    }
    result[i] = '\0';
} // end of subString(...) helper method

string substring ( string * str, int beginIndex, int endIndex)
{
    // preconditions
    assert(str != NULL);
    assert(str->content != NULL);
    assert(beginIndex >= 0 && beginIndex <= endIndex);
    assert(endIndex < str->length);

    int index = 0; // index for traverse the result array
    string *result = malloc(sizeof(string));
    result->length = endIndex - beginIndex; // length of the substring
    result->content = malloc( result->length+1 * sizeof(char) ); // add 1 for the string terminator

    for(int i=beginIndex; i <= endIndex; i++){
        result->content[index++] = str->content[i];
    }

    result->content[index] = '/0'; // string termintor

    // postconditions
    assert(result->content != NULL);
    assert(strlen(result->content) == result->length);
    
    return result;
}


/** Converted methods */
void testDistance(char s[], char t[], int expected) {
    int distance = levenshtein(s, t);

    if (distance == expected) {
        printf("Passed! You can get from '%s' to '%s' in %u moves\n", s, t, expected);
    }
    else {
        printf("FAILED: I thought it would take %u moves, but appearantly it takes %u moves to get from '%s' to '%s'.\n", expected, distance, s, t);
        testsFailed++;
    }
    testsExecuted++;
} // end of testDistance(...) method

int levenshtein(char s[], char t[]) {
    int cost;
    int distance;
    char deleteS[strlen(s) + 1];
    char deleteT[strlen(t) + 1];

    if (strlen(s) == 0) {
        distance = strlen(t);
    }
    else if (strlen(t) == 0) {
        distance = strlen(s);
    }
    else {
        if (s[0] == t[0]) {
            cost = 0;
        }
        else {
            cost = 1;
        }

        subString(s, deleteS, 1, strlen(s) - 1);
        subString(t, deleteT, 1, strlen(t) - 1);

        assert(strlen(deleteS) == strlen(s) - 1);
        assert(strlen(deleteT) == strlen(t) - 1);

        assert(endsWith(s, deleteS) == TRUE);
        assert(endsWith(t, deleteT) == TRUE);

        int new[] = { levenshtein(deleteS, t) + 1,
                            levenshtein(s, deleteT) + 1,
                            levenshtein(deleteS, deleteT) + cost };
        distance = minimum(new);
    }

    return distance;
} // end of levenstein(...) method

int minimum(int minimum[]) {
    int min = 0;
    assert(MAX > 0);

    if (MAX > 0) {
        min = minimum[0];

        for (int i = 1; i < MAX; i++) {
            if (minimum[i] < min) {
                min = minimum[i];
            }
        }
    }

    return min;
} // end of minimum(...) method