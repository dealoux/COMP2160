//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 3 question 2
// 
// REMARKS: This module is the test suite for the Set ADT
//
//-----------------------------------------

#include <stdio.h>
#include <assert.h>
#include "set.h"

static int testsFailed = 0;
static int testsExecuted = 0;

void deleteSetSH(Set*, char*);
void deleteSetMemValidate();
void insertTest(Set*, int, char*, Boolean);
void removeTest(Set*, int, char*, Boolean);
void areEqualTest(Set*, Set*, char*, char*, Boolean);
void areDisjointTest(Set*, Set*, char*, char*, Boolean);
void unionOfTest(Set*, Set*, char*, char*, Set*);
void symmetricDifferenceOfTest(Set*, Set*, char*, char*, Set*);
void test();

// Main function, calls the test suite
int main(void)
{
    test();

    printf("The program terminated normally\n");
	return 0;
} // end of main()

// Test suite
void test() {
    assert(validateMemUse());

    printf("Creating sets.\n");
    Set* set1 = newSet();
    Set* set2 = newSet();
    Set* set3 = newSet();

    printf("\nINSERTION TESTS\n");
    insertTest(set1, 1, "1", true);
    insertTest(set1, 2, "1", true);
    insertTest(set1, 3, "1", true);
    
    insertTest(set2, 2, "2", true);
    insertTest(set2, 1, "2", true);
    insertTest(set2, 3, "2", true);
    
    insertTest(set3, 4, "3", true);
    insertTest(set3, 5, "3", true);
    insertTest(set3, 6, "3", true);
    insertTest(set3, 7, "3", true);
    insertTest(set3, 8, "3", true);

    // duplicates
    printf("\nThese should fail as they are duplicates\n");
    insertTest(set1, 3, "1", false);
    insertTest(set1, 1, "1", false);
    insertTest(set1, 2, "1", false);
    insertTest(set2, 2, "2", false);
    insertTest(set2, 1, "2", false);
    insertTest(set2, 3, "2", false);

    
    printf("\n\nEQUAL TESTS\n");
    areEqualTest(set1, set2, "1", "2", true);
    insertTest(set2, 5, "2", true);
    areEqualTest(set1, set2, "1", "2", false);
    areEqualTest(set1, set3, "1", "3", false);

    
    printf("\n\nDISJOINT TESTS\n");
    areDisjointTest(set1, set2, "1", "2", false);

    areDisjointTest(set1, set3, "1", "3", true);
    areDisjointTest(set1, set2, "1", "2", false);
    areDisjointTest(set2, set3, "2", "3", false);

    
    printf("\n\nREMOVE TESTS\n");
    printf("\nRemove the first item\n");
    removeTest(set3, 4, "3", true);
    printf("\nRemove the last item\n");
	removeTest(set3, 8, "3", true);
    printf("\nRemove in between item\n");
    removeTest(set3, 6, "3", true);
    printf("\nRemoving not found items, these should fail\n");
    removeTest(set1, 4, "1", false);
    removeTest(set3, 3, "3", false);


    printf("\n\nCLEAR TESTS\n");
    deleteSetSH(set1, "1");
    deleteSetSH(set2, "2");
    deleteSetSH(set3, "3");
    deleteSetMemValidate();
    set1 = newSet();
    set2 = newSet();
    set3 = newSet();


    printf("\n\nUNION TEST\n");
    insertTest(set1, 1, "1", true);
    insertTest(set1, 2, "1", true);
    insertTest(set1, 3, "1", true);
    
    insertTest(set2, 2, "2", true);
    insertTest(set2, 4, "2", true);
    insertTest(set2, 5, "2", true);
    insertTest(set2, 6, "2", true);
    
    insertTest(set3, 1, "3", true);
    insertTest(set3, 2, "3", true);
    insertTest(set3, 3, "3", true);
    insertTest(set3, 4, "3", true);
    insertTest(set3, 5, "3", true);
    insertTest(set3, 6, "3", true);

    unionOfTest(set1, set2, "1", "2", set3);

    printf("\n\nSYMMETRIC DIFFERENCE TEST\n");
    insertTest(set1, 4, "1", true);
    removeTest(set3, 2, "2", true);
    removeTest(set3, 4, "2", true);
    symmetricDifferenceOfTest(set1, set2, "1", "2", set3);

    printf("\nTotal number of tests executed: %u\n", testsExecuted);
    printf("Number of tests passed: %u\n", (testsExecuted - testsFailed));
    printf("Number of tests failed: %u\n", testsFailed);
} // end of test()

// This function tests the functionality of the insertItem() function and reports appropriately
void insertTest(Set* set, int item, char *setID, Boolean expected) {
    printf("\nInserting: %d into set %s\n", item, setID);
    Boolean result = insertItem(set, item);

    if (result == expected) {
        printf("SUCCESS: the result is %s as expected\n", result ? "true" : "false");
    }
    else {
        printf("FAILED: the result is expected to be %s but instead is %s\n", expected ? "true" : "false", result ? "true" : "false");
        testsFailed++;
    }
    
    testsExecuted++;
} // end of insertTest()

// This function tests the functionality of the removeItem() function and reports appropriately
void removeTest(Set* set, int item, char *setID, Boolean expected) {
    printf("\nRemoving: %d from set %s\n", item, setID);
    Boolean result = removeItem(set, item);

    if (result == expected) {
        printf("SUCCESS: the result is %s as expected\n", result ? "true" : "false");
    }
    else {
        printf("FAILED: the result is expected to be %s but instead is %s\n", expected ? "true" : "false", result ? "true" : "false");
        testsFailed++;
    }

    testsExecuted++;
} // end of removeTest()

// This function tests the functionality of the areEqual() function and reports appropriately
void areEqualTest(Set* setA, Set* setB, char *setID1, char *setID2, Boolean expected) {
    assert(!validateMemUse());

    printf("\nTesting the eqality between set %s and set %s\n", setID1, setID2);
    Boolean result = areEqual(setA, setB);

    if (result == expected) {
        printf("SUCCESS: the result is %s as expected\n", result ? "true" : "false");
    }
    else {
        printf("FAILED: the result is expected to be %s but instead is %s\n", expected ? "true" : "false", result ? "true" : "false");
        testsFailed++;
    }

    testsExecuted++;
} // end of areEqualTest()

// This function tests the functionality of the areDisjoint() function and reports appropriately
void areDisjointTest(Set* setA, Set* setB, char *setID1, char *setID2, Boolean expected) {
    assert(!validateMemUse());

    printf("\nnTesting the disjoint between set %s and set %s\n", setID1, setID2);
    Boolean result = areDisjoint(setA, setB);

    if (result == expected) {
        printf("SUCCESS: the result is %s as expected\n", result ? "true" : "false");
    }
    else {
        printf("FAILED: the result is expected to be %s but instead is %s\n", expected ? "true" : "false", result ? "true" : "false");
        testsFailed++;
    }

    testsExecuted++;
} // end of areDisjointTest()

// This function tests the functionality of the unionOf() function and reports appropriately
// Set A & B being the two sets being tested on, set C is the expected resulting set
void unionOfTest(Set* setA, Set* setB, char *setID1, char *setID2, Set* setC) {
    Set *result;
    assert(!validateMemUse());
    printf("\nTesting the union of set %s and set %s\n", setID1, setID2);
    result = unionOf(setA, setB);

    if (areEqual(result, setC)) {
        printf("SUCCESS: the unionOf() method is working as intended\n");
    }
    else {
        printf("FAILED: the resultitng set is not as expected\n");
        testsFailed++;
    }

    testsExecuted++;
} // end of unionOfTest()

// This function tests the functionality of the symmetricDifferenceOf() function and reports appropriately
// Set A & B being the two sets being tested on, set C is the expected resulting set
void symmetricDifferenceOfTest(Set* setA, Set* setB, char *setID1, char *setID2, Set* setC) {
    Set *result;
    assert(!validateMemUse());
    printf("\nTesting the symmetric difference of set %s and set %s\n", setID1, setID2);
    result = symmetricDifferenceOf(setA, setB);

    if (areEqual(result, setC)) {
        printf("SUCCESS: the symmetricDifferenceOf() method is working as intended\n");
    }
    else {
        printf("FAILED: the resultitng set is not as expected\n");
        testsFailed++;
    }

    testsExecuted++;
} // end of symmetricDifferenceOf()

// This function is a shorthand for deleteSet() method
void deleteSetSH(Set* set, char* setID){
    printf("Deleting set %s\n", setID);
    deleteSet(set);
} // end of deleteSetSH()

// This function tests the functionality of the set destructor and reports appropriately
void deleteSetMemValidate() {
    printf("\nValidating memory\n");

    if (validateMemUse()) {
        printf("SUCCESS in clearing the memory\n");
    }
    else {
        printf("FAILED to clear the memory\n");
        testsFailed++;
    }
    testsExecuted++;
} // end of deleteSetTest()