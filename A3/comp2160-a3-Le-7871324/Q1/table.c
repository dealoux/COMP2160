//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 3 question 1
// 
// REMARKS: This program is a implement of the table ADT, while also applying the principles of Design by Contract
//
//-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "table.h"

// node definition
typedef struct Node node;

struct Node
{
	int   number;
	node  *next;
};

static node *top = NULL;
// used to track where we are for the list traversal methods
static int index = 0;
// used to track where we are for the list traversal methods
static node *traverseNode = NULL;

// add an element to the table
// Return TRUE if the item is in the table
// Return FALSE if the item is *not* in the table
Boolean insertItem( int item ){
	Boolean result = false;
	node *curr;
	node *toAdd;
	node dummy;

	if(!search(item)){
		toAdd = malloc(sizeof(node));
		assert(toAdd);
		toAdd->number = item;

		curr = &dummy;
		dummy.next = top;

		while(curr->next && curr->next->number < toAdd->number){
			curr = curr->next;
		}

		toAdd->next = curr->next;
    	curr->next = toAdd;
    	top = dummy.next;

		result = true;
	}

	return result;
} // end of insertItem()

// removes the int from the table
Boolean removeItem( int item ){
	Boolean result = false;
	node *prev;
	node *toRemove;

	if(top){
		if(search(item)){
			if(index > 0){
				// get the previous node
				prev = top;
				for(int i=1; i<index; i++){
					prev = prev->next;
				}

				toRemove = prev->next;
				prev->next = toRemove->next;
			}

			else{
				assert(index == 0);
				toRemove = top;
				top = top->next;
			}

			toRemove->next = NULL;
			free(toRemove);

			result = true;
		}
	}

	return result;
} // end of removeItem()

// empty the table so that we clear all memory and can start a fresh table
void clearTable( ){
	node *curr = top;
	node *temp = NULL;

	while(curr){
		// flip order to see it blow up...
		temp = curr;
		curr = curr->next;

		free( temp );
  	}

  	top = NULL;
	assert(!top);
	assert(!curr);
} // end of clearTable()

// tells us whether or not the given item is in the table
Boolean search( int item ){
	Boolean result = false;

	node *curr = top;
	index = 0;
	while(curr){
		if(curr->number == item){
			result = true;
			break;
		}
		else if(curr->number < item){
			curr = curr->next;
			index++;
		}
		else{
			assert(curr->number > item);
			index = -1;
			break;
		}
	}

	return result;
} // end of search

// table iterators
// Return TRUE if item was assigned
// Return FALSE if item was *not* assigned
Boolean firstItem( int * const item ){
	Boolean result = false;

  	if(top){
    	*item = top->number;
    	traverseNode = top->next;
		result = true;
  	}
	
  	return result;
} // end of firstItem()

Boolean nextItem( int * const item ){
	Boolean result = false;

  	if(traverseNode){
    	*item = traverseNode->number;
    	traverseNode = traverseNode->next;
		result = true;
  	}
  
  	return result;
} // end of nextItem()