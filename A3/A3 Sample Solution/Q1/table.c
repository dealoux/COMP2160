#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "table.h"

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

// Linked list node definition
typedef struct NODE Node;

struct NODE
{
    int num;
    Node *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

static Node *top = NULL;
 
// used to track where we are for the list traversal methods
static Node *traverseNode = NULL;

// used to track testing values
static int num_nodes = 0;
static int num_traversals = 0;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------


static void validateTable()
{
#ifndef NDEBUG
    Node *curr = top;
    int node_count = 0;
    int prev_num;
#endif

    if ( num_nodes == 0 )
        assert( top == NULL );
    else if ( num_nodes == 1 )
        assert( top->next == NULL );
    else // num_nodes > 1
        assert( top != NULL && top->next != NULL );

// There's a really interesting discussion to be had here on whether or not we should traverse the list.
// Here, the number of elements is unbounded and as such a full traversal will take an indeterminate amount
// of time. If you do include the traversal, you *must* ensure that it gets compiled out when NDEBUG is defined.
// Our leak testing code actually gets completely demolished in terms of performance since we're just inserting
// and removing, and iterating over the entire list whenever we do an insertion or deletion.
#ifndef NDEBUG
    while ( curr )
    {
        node_count++;
        prev_num = curr->num;
        curr = curr->next;
        
        if ( curr )
        {
            // checking that we're in order (Note: less-than rather than less-than-or-equal implies no duplicates)
            assert( prev_num < curr->num );
            prev_num = curr->num;
        }
    }
    assert( node_count == num_nodes );
#endif
// It's also worth noting that we do actually test the structure of the list via a test/use of the iterator functions.

}

// add an element to the beginning of the linked list
Boolean insertItem( int const new_num )
{
    Boolean rc = true;
    Boolean alreadyPresent = false;
    Node *newNode = NULL;
    Node *curr = top;
    Node *prev = NULL;
  
    validateTable();

    // first let's decide if we even *need* to insert it:
    alreadyPresent = search ( new_num );

    if ( !alreadyPresent )
    {
        newNode = malloc( sizeof( Node ) );
        assert( newNode != NULL );

        if ( newNode )
        {
            newNode->num = new_num;

            assert( curr != NULL || ( curr == NULL && top == NULL ) );

            if ( curr == NULL && top == NULL )
            {
                // this is the first item we're inserting into the list, so just make it top
                top = newNode;
                newNode->next = NULL;
            }
            else
            {
                assert( curr != NULL );
                
                // now we need to figure out **where** to add it
                while ( curr && curr->num < new_num )
                {
                    prev = curr;
                    curr = curr->next;
                }

                if ( prev == NULL )
                {
                    // inserting to the top of the list
                    assert( curr == top );
                    
                    newNode->next = top;
                    top = newNode;
                }
                else
                {
                    assert( curr != top );

                    // inserting between elements.
                    prev->next = newNode;
                    newNode->next = curr;
                }
            }

            num_nodes++;
            rc = true;
            // make sure the list is stable
            validateTable();
        }
        else
        {
            rc = false;
        }
    }
  
    return rc;
}


// empty the Table so that we clear all memory and can start a fresh list
void clearTable()
{
    Node *curr = top;
  
    validateTable();
  
    while ( top != NULL )
    {
        top = top->next;
        free( curr );
    
        curr = top;
    
        num_nodes--;
    }
  
    validateTable();
}

// remove an element with the given int
Boolean removeItem( int const target )
{
    Boolean deleted = false;
    Node *curr = top;
    Node *prev = NULL;
  
    validateTable();
  
    while ( curr != NULL && curr->num != target )
    {
        prev = curr;
        curr = curr->next;
    }
    
    if ( curr != NULL )
    {
        if ( prev != NULL )
            prev->next = curr->next;
        else
            top = curr->next;
      
        free( curr );
        curr = NULL;
      
        deleted = true;
        num_nodes--;
      
        // make sure the list is stable
        validateTable();
    }
  
    return deleted;
}

// tells us whether or not the given int is in the list
Boolean search( int const target )
{
    Boolean found = false;
    Node *curr = top;
    int search_count = 0;     // how far in the list do we go?
  
    validateTable();
  
    while ( curr != NULL && !found )
    {
        if ( curr->num == target )
        {
            found = true;
        
            // make sure we're still in our list...
            assert( search_count <= num_nodes );
        }
        else
        {
            curr = curr->next;
            search_count++;
        }
    }
  
    // if it's not found then we should be at the end of the list
    assert( found || (search_count == num_nodes) );
  
    return found;
}


// starts a list traversal by getting the data at top
Boolean firstItem( int * const target )
{
    Boolean result = false;
  
    validateTable();
  
    if ( top != NULL )
    {
        traverseNode = top->next;
        *target = top->num;
        result = true;
    
        num_traversals = 1;
        // make sure we're still in our list...
        assert( num_traversals <= num_nodes );
    }
  
    return result;
}


// gets the data at the current traversal node and increments the traversal
Boolean nextItem( int * const target )
{
    Boolean result = false;
  
    validateTable();
  
    // no need to go past the end of the list...
    if ( traverseNode != NULL )
    {
        *target = traverseNode->num;
        result = true;

        traverseNode = traverseNode->next;
    
        num_traversals++;
        // make sure we're still in our list...
        assert( num_traversals <= num_nodes );
    }
  
    return result;
}
