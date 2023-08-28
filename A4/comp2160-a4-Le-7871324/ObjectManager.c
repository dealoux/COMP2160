//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 4
// 
// REMARKS: This module is the implementation of the Object Manager
//
//-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ObjectManager.h"

typedef struct MEMBLOCK memblock;
struct MEMBLOCK
{
    int startPos; // start position of this object
    ulong size; // size of this object
    Ref ref; // the reference to identify this object
    int refCount; // total count of references to this object 
    memblock* next; // pointer to the next memory block
};

// Object manager variables
static Ref nextRef = 1; // keep track of the next reference
static int freeIndex = 0; // keep track of the current starting free index of the current buffer
static uchar buffer1[MEMORY_SIZE]; 
static uchar buffer2[MEMORY_SIZE];
static uchar *currBuffer = buffer1; // pointer to the current buffer, buffer1 intitially

// Linked list variables
static memblock *head; // pointer to the 1st block of the list
static memblock *tail; // pointer to the last block of the list
static int numBlocks = 0; // total number of memory blocks inside the linked list


/* Helper functions */
// This function returns the pointer to a newly initialized memory block using the given data
static memblock* createBlock(int pos, ulong size, Ref ref, memblock *next) {
    // pre-conditions, bounds checks
    assert(size > 0);
    assert(ref >= 0 && ref < nextRef);
    assert(pos >= 0 && pos < MEMORY_SIZE);

    memblock* newBlock = (memblock*)malloc(sizeof(memblock));
    assert(newBlock != NULL);

    if(newBlock != NULL){
        newBlock->startPos = pos; 
        newBlock->size = size;
        newBlock->ref = ref;
        newBlock->next = next;
        newBlock->refCount = 1;
    }

    return newBlock;
} // end of createBlock() helper function

// This function loops through the linked list to try and find the memblock that matches the given reference
// returns the pointer to that memblock if found or NULL otherwise
static memblock* getBlock(Ref ref){
    assert(ref >= 0 && ref < nextRef); // pre-condition

    memblock *result = NULL;
    memblock *curr = head;

    // binary search through the linked list
    while(curr){
        if(curr->ref == ref){
            result = curr;
            break;
        }
        curr = curr->next;
    }

    return result;
} // end of getBlock() helper function

// This function loops through the linked list to try and find the memblock that matches the given reference
// returns the pointer to that memblock if found or NULL otherwise
static void removeBlock(memblock *toRemove){
    // pre-conditions
    assert(toRemove != NULL);

    memblock *prev;
    memblock *curr;
    
    // checks if the to be removed block is the 1st block
    if(toRemove == head){
        head = head->next;
        memset(&currBuffer[toRemove->startPos], 0x00, 0); // clear the memory from the buffer
        free(toRemove);
    }
    // else it's not the head block
    else{
        prev = head;
        curr = head->next;

        while(curr != toRemove){
            prev = curr;
            curr = curr->next;
        }

        // check if the to be removed block is the last block
        if(curr == tail){
            assert(tail == toRemove);
            tail = prev;
        }

        assert(curr == toRemove);
        prev->next = curr->next;
        memset(&currBuffer[curr->startPos], 0x00, 0); // clear the memory from the buffer
        free(curr);
    }

    numBlocks--;
} // end of removeBlock() helper function

// This function uses the Mark and Sweep Defragmenting strategy by compressing free spaces between allocated blocks by moving them between 2 buffers
// This results in a large free space that has been combined, allowing for more memory blocks to be allocated
static void compact() {
    int total_size = freeIndex; // current total bytes in the memory before defragmentation
    freeIndex = 0; // reset the starting index back to 0 since we now switching buffer
    int total_objects = 0; // objects count is currently 0
    uchar *newBuffer = (currBuffer == buffer1) ? buffer2 : buffer1; // point to the remaining buffer
    memblock *curr = head;

    // iterate through the linked list and move each block from the current buffer to its new position in the new buffer
    while (curr != NULL){
        // invariant, bounds checks
        assert(curr->startPos >= 0 && curr->startPos < MEMORY_SIZE);
        assert(curr->size >= 0 && (curr->size + freeIndex) < MEMORY_SIZE);

        // copy the block to its new position in the new buffer
        memcpy(&newBuffer[freeIndex], &currBuffer[curr->startPos], curr->size);
        curr->startPos = freeIndex;
        freeIndex += curr->size;

        total_objects++; // keep track of the object counts
        curr = curr->next;
    }

    // prints the statics for debugging purposes
    printf("Garbage collector statistics:\n");
    printf("objects: %d   bytes in use : %d   freed : %d\n", total_objects, freeIndex, total_size - freeIndex);
    
    currBuffer = newBuffer;
} // end of compact() helper function


/* Interface functions */
// This function try to allocate a new memory block with the given size in the current buffer, returns its reference if success or NULL otherwise 
// If there's no space left, it would try to call compact() to free up some if there's any
// If there's free space available now, try allocating again or otherwise inform the user 
Ref insertObject(ulong size){
    // pre-conditions
    assert(size > 0);

    Ref newRef = NULL_REF;
    memblock *newBlock = NULL;

    // check if there's any garbage to collect
    if(size + freeIndex >= MEMORY_SIZE){
        compact();
    }

    // begin insertion if there's space
    if(size + freeIndex < MEMORY_SIZE){
        newRef = nextRef++;
        newBlock = createBlock(freeIndex, size, newRef, NULL);

        // add to the end of the linked list if not empty
        if(tail != NULL){
            tail->next = newBlock;
        }

        tail = newBlock;

        // 1st block
        if(numBlocks == 0){
            head = newBlock;
        }

        numBlocks++;

        // clear and set the memory at the freeIndex with the given size and update the variable accordingly
        memset(&currBuffer[freeIndex], 0x00, size);
        freeIndex += size;
        assert(freeIndex <= MEMORY_SIZE); // post-condition, bound check
    }
    // else the memory is full even after calling compact(), inform the user
    else{
        printf("Unable to successfully complete memory allocation request, the memory is full!\n");
    }

    return newRef;
} // end of insertObject()

// This function returns a pointer to the object being requested given by the reference id if found
// Otherwise it will terminate the program as the user is trying to reference a non-existing block of memory
void* retrieveObject(Ref ref) {
    void* result = NULL;
    memblock *block = getBlock(ref);

    // only get the result memory in the buffer if the block is found
    if(block != NULL){
        result = &currBuffer[block->startPos]; // assign the resulting block
    }
    // else handle the error
    else{
        printf("Invalid reference exception with reference %ld, the program will now terminate\n", ref);
        exit(1);
    }

    return result;
} // end of retrieveObject() function

// This function calls getBlock() with the given reference, then increases that block's ref count by 1 if found
void addReference(Ref ref) {
    memblock *block = getBlock(ref);

    if(block != NULL){
        block->refCount++;
    }
} // end of addReference() function

// This function calls getBlock() with the given reference, then decreases that block's ref count by 1 if found
// Then checks whether or not the block should then be removed for free space
void dropReference(Ref ref) {
    memblock *block = getBlock(ref);

    if(block != NULL){
        assert(block->refCount > 0);
        block->refCount--;

        if(block->refCount == 0){
            removeBlock(block);
        }
    }
} // end of dropReference() method

// This function initializes the linked list
void initPool(){
    head = tail = NULL;
    numBlocks = 0;
} // end of initPool()

// This function clears all the nodes from the linked list and then re-initialize object manager
void destroyPool(){
    memblock *curr = head;
    memblock *temp;

    while(curr != NULL){
        temp = curr;
        curr = curr->next;
        free(temp);
    }

    initPool(); // re-initializes the object manager
} // end of destroyPool()

void dumpPool() {
    memblock* curr = head;

    // iterate through the linked list and print out every memory block
    while (curr != NULL) {
        printf("Block %lu, starting address = %u, size of %lu, referencing count of %d\n", curr->ref, curr->startPos, curr->size, curr->refCount);
        curr = curr->next;
    }

    printf("Next index = %d\n", freeIndex); // also print out the current available freeIndex
} // end of dumpPool() function
