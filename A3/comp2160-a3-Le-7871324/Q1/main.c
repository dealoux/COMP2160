//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 3, question 1
// 
// REMARKS: This module is the testing suite for the Table ADT
//
//-----------------------------------------

#include <stdio.h>
#include "table.h"

static int testsFailed = 0;
static int testsExecuted = 0;

void insertTest(int);
void removeTest(int);
void searchTest(int);
void clearTableTest();
void traversePrint();
void test();

// Main function, calls the test suite
int main(void){
	test();

	printf("The program terminated normally\n");
	return 0;
} // end of main()

// Test suite
void test() {
    // Empty table edge case
    printf( "EDGE CASE: Testing on an empty table, these should fail\n" );
	searchTest(8);
	removeTest(8);
    traversePrint();

    // Insertion
    printf("\nINSERTION TESTS\n");
    insertTest(10);
	insertTest(7);
	insertTest(8);
	insertTest(15);
    insertTest(5); // first item
    traversePrint();
    printf("\nThese should fail as they are duplicates\n");
    // duplicates
	insertTest(8);
	insertTest(5);
	insertTest(15);
	insertTest(10);
    insertTest(7);
    traversePrint();
    // new item
    printf("\nNormal insertion again\n");
    insertTest(12);
    insertTest(34); // last item
    traversePrint();

    // Search
    printf("\n\nSEARCH TESTS\n");
    printf("\nSearch for the first item\n");
    searchTest(5);
    printf("\nSearch for the last item\n");
	searchTest(34);
    printf("\nSearch for in between items\n");
	searchTest(12);
    searchTest(10);
    printf("\nSearch for non existing items, these should fail\n");
	searchTest(6);
    searchTest(14);
    searchTest(31);
    traversePrint();

    // Remove
    printf("\n\nREMOVE TESTS\n");
    printf("\nRemove the first item\n");
    removeTest(5);
    traversePrint();
    printf("\nRemove the last item\n");
	removeTest(34);
    traversePrint();
    printf("\nRemove in between items\n");
	removeTest(12);
    traversePrint();
    removeTest(10);
    traversePrint();
    printf("\nRemove non existing items, these should fail\n");
	removeTest(6);
    removeTest(14);
    removeTest(31);
    traversePrint();

    // Clearing and retrying
    printf("\n\nCLEARING TEST\n");
    clearTableTest();
    traversePrint();
    printf("Try insertion, search and remove again\n");
    insertTest(10);
	insertTest(7);
	insertTest(8);
    traversePrint();
    searchTest(10);
    searchTest(5);
    removeTest(6);
    removeTest(8);
    traversePrint();
    
    // Summary of the test
    printf("\nTotal number of tests executed: %u\n", testsExecuted);
    printf("Number of tests passed: %u\n", (testsExecuted - testsFailed));
    printf("Number of tests failed: %u\n", testsFailed);
} // end of test()

// This function tests the functionality of the insertItem() function and reports appropriately
void insertTest(int num) {
    printf("Inserting \"%d\"\n", num);

    if (insertItem(num) && search(num)) {
        printf("SUCCESS in inserting \"%d\"\n", num);
    }
    else {
        printf("FAILED to insert \"%d\"\n", num);
        testsFailed++;
    }

    testsExecuted++;
} // end of insertTest()

// This function tests the functionality of the removeItem() function and reports appropriately
void removeTest(int num) {
    printf("Removing \"%d\" from the table\n", num);
    
    if (removeItem(num) && !search(num)) {
        printf("SUCCESS in removing \"%d\"\n", num);
    }
    else {
        printf("FAILED to remove \"%d\"\n", num);
        testsFailed++;
    }

    testsExecuted++;
} // end of removeTest()

// This function tests the functionality of the searchItem() function and reports appropriately
void searchTest(int num) {
    printf("Searching for \"%d\" in the table table\n", num);

    //printf("Size of the table: %d\n", size(table));

    if (search(num)) {
        printf("SUCCESS \"%d\" is found in the table\n", num);
    }
    else {
        printf("FAILED to find \"%d\" in the table\n", num);
        testsFailed++;
    }

    testsExecuted++;
}

// This function tests the functionality of the searchItem() function
// returns true if it passed
// otherwise reports and return false
Boolean firstItemTest(int * const num){
    Boolean result = firstItem(num);

    if(!result){
        printf("FAILED to get the first item of the table\n");
        testsFailed++;
    }

    testsExecuted++;
    return result;
} // end of firstItemTest()

// This function tests the functionality of the clearTable() function and reports appropriately
void clearTableTest() {
    int num;

    printf("\nClearing the table\n");
    clearTable();

    if( firstItem(&num) ){
        printf("FAILED to clear the table, the 1st item still exist\n");
        testsFailed++;
    }
    else{
        printf("SUCCESS in clearing the table\n");
    }

    testsExecuted++;
} // end of clearTableTest()

// This function traverses and prints all the item(s) of the table
void traversePrint(){
    int num;

    printf("\nPrinting the table\n");

    if( firstItemTest(&num) ){
        do{
            printf("%d ", num);
        } while ( nextItem(&num) );    
    }

    printf("\n");
} // end of traversePrint()