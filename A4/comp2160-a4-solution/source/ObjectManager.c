#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ObjectManager.h"
#include <assert.h>

// tracks the next reference to use, we start at 1 so we can use 0 as the NULL reference
// we just continue counting since we have lots here and aren't worried about overflow for this assignment
static Ref nextRef = 1;

// A Memblock holds the relevent information associated with an allocated block of memory by our memory manager
typedef struct MEMBLOCK MemBlock;


// information needed to track our objects in memory
struct MEMBLOCK
{
  int numBytes;    // how big is this object?
  int startAddr;   // where the object starts
  Ref ref;         // the reference used to identify the object
  int count;       // the number of references to this object
  MemBlock *next;  // pointer to next block.  Blocks stored in a linked list.
};


// The blocks are stored in a linked list where the start of the list is pointed to by memBlockStart.
static MemBlock *memBlockStart; // start of linked list of blocks allocated
static MemBlock *memBlockEnd;   // end of linked list of blocks allocated
static int numBlocks;            // number of blocks allocated

// our buffers.  This is where we allocate memory from.  One of these is always the current buffer.  The other is used for swapping
//during compaction stage.

static unsigned char buffer1[MEMORY_SIZE];
static unsigned char buffer2[MEMORY_SIZE];

// points to the current buffer containing our data
static unsigned char *currBuffer = buffer1;

// points to the location of the next available memory location
static int freeIndex = 0;

//initialize the block pointed to by thePtr
static void initMemBlock( MemBlock * const thePtr, const int bytes, const int addr, const Ref ref, MemBlock * const theNext )
{
  assert( bytes > 0 );
  assert( ref >= 0 && ref < nextRef );
  assert( addr >= 0 && addr < MEMORY_SIZE );
  
  thePtr->numBytes = bytes;
  thePtr->startAddr = addr;
  thePtr->ref = ref;
  thePtr->next = theNext;
  
  thePtr->count = 1;
}

// a better way to do this would be to hash our reference number into a table use the MAD technique
static MemBlock *find( const Ref ref )
{
  
  MemBlock *index = NULL;
  MemBlock *ptr;
  
  ptr = memBlockStart;  //start at the beginning of the linked list and search for node containing reference id ref.
  while ((ptr != NULL)  && (index == NULL))
  {
    if (ptr->ref == ref)
    {
      index = ptr;
    }
    else
    {
      ptr=ptr->next;
    }
  }
  
  return index;
  
}

// performs required setup
void initPool()
{
  memBlockStart = NULL;
  memBlockEnd = NULL;
  numBlocks = 0;
}

// performs required clean up
void destroyPool()
{
  //we want to delete all nodes from the linked list.
  MemBlock *curr;
  MemBlock *next;
  
  curr = memBlockStart;
  while (curr != NULL)
  {
    next = curr->next;
    free(curr);
    curr = next;
  }
  
  memBlockStart = NULL;
  memBlockEnd = NULL;
  numBlocks = 0;
}

// performs our garbage collection
void compact()
{
  
  // decide where we're going to put all the objects
  unsigned char *new_buffer = (currBuffer==buffer1) ? buffer2 : buffer1;
  int byte_count = freeIndex;
  int object_count = 0;
  
  freeIndex = 0;
  MemBlock *curr;  //used for iterating thru linked list
  
  curr = memBlockStart;
  while (curr != NULL)
  {
    // copy the data and update our index
    assert( curr->startAddr >= 0 && curr->startAddr < MEMORY_SIZE );
    assert( curr->numBytes >= 0 && curr->numBytes < (MEMORY_SIZE-freeIndex) );
    
    memcpy( &new_buffer[freeIndex], &currBuffer[curr->startAddr], curr->numBytes );
    curr->startAddr = freeIndex;
    freeIndex += curr->numBytes;
    curr = curr->next;
    object_count++;
  }
  
  // print out our statistics
#ifndef NDEBUG
  printf( "\nGarbage collector statistics:\n" );
  printf( "objects: %d   bytes in use: %d   freed: %d\n\n", object_count, freeIndex, (byte_count-freeIndex) );
#endif
  // done, update our buffer pointer
  currBuffer = new_buffer;
}

// Adds the given object into our buffer. It will fire the garbage collector as required.
// We always assume that an insert always creates a new object...
// On success it returns the reference number for the block allocated.
// On failure it returns NULL_REF (0)
Ref insertObject( const int size )
{
  Ref new_ref = NULL_REF;
  
  assert( size > 0 );
  
  // start by seeing if we need to garbage collect
  if ( size >= (MEMORY_SIZE-freeIndex) )
    compact();
  
  // only add the data if we have space
  if ( size < (MEMORY_SIZE-freeIndex) )
  {
    // add the index information
    
    MemBlock *ptr = (MemBlock *)malloc(sizeof(MemBlock));
    assert (ptr != NULL);
    if (ptr != NULL)
    {
      new_ref = nextRef++;
      initMemBlock( ptr, size, freeIndex, new_ref, NULL );
      numBlocks++;
      
      //add block/node to end of list
      if (memBlockEnd != NULL)
        memBlockEnd->next = ptr;
      memBlockEnd = ptr;
      
      //there are no blocks allocated.  This is the first one.
      if (numBlocks == 1)
      {
        memBlockStart = ptr;
      }
      
      // clear the data in our memory
      memset( &currBuffer[freeIndex], 0x00, size );
      freeIndex += size;
      
      assert( freeIndex <= MEMORY_SIZE );
    }
    
  }
  
  // we've tried everything, there's no hope!
  // maybe we don't need to print a message.
  else
  {
#ifndef NDEBUG
    printf( "Unable to successfully complete memory allocation request.\n" );
#endif
  }
  return new_ref;
}

// returns a pointer to the object being requested
void *retrieveObject( const Ref ref )
{
  MemBlock *index;
  void *object;
  
  assert( ref >= 0 && ref < nextRef );
  index = find(ref);
  
  if (index != NULL)
  {
    object = &currBuffer[index->startAddr];
  }
  
  // otherwise we have an error that will cause a crash!
  else
  {
    printf( "Invalid reference exception with reference %ld, terminating process.\n", ref );
    exit( EXIT_FAILURE );
    object = NULL;
  }
  
  return object;
}

// update our index to indicate that we have another reference to the given object
void addReference( const Ref ref )
{  
  MemBlock *index;
 
  assert( ref >= 0 && ref < nextRef );
  index = find( ref );
  
  if (index != NULL)
  {
    index->count = index->count + 1;
  }
}

// update our index to indicate that a reference is gone
void dropReference( const Ref ref )
{  
  MemBlock *index;
  MemBlock *prev;
  MemBlock *curr;
  
  assert( ref >= 0 && ref < nextRef );
  index = find( ref );
  
  if (index != NULL)
  {
    index->count = index->count - 1;
    if (index->count == 0)
    {
      //need to remove this node from the list
      if (index == memBlockStart) //it's the first node.
      {
        memBlockStart = memBlockStart->next;
        free(index);
      }
      
      else  //it's not first node.
      {
        prev = memBlockStart;
        curr = memBlockStart->next;
        while (curr != index)
        {
          prev = curr;
          curr = curr->next;
        }
        //see if it's the last node we are deleting.
        if (curr == memBlockEnd)
          memBlockEnd= prev;
        
        prev->next = index->next;
        
        free(index);
      }
      
      numBlocks--;
      
      if (numBlocks <= 1)
        memBlockEnd = memBlockStart;
      
    }
  }
}


void dumpPool()
{
  MemBlock *curr;
  
  curr = memBlockStart;
  
  while (curr != NULL)
  {
    printf( "refid = %lu, start addr = %d, num bytes = %d, reference count = %d\n", curr->ref, curr->startAddr, curr->numBytes, curr->count );
    curr=curr->next;
  }
  printf( "next available index = %d\n", freeIndex );  
}