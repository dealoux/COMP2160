#include <assert.h>
#include <stdio.h>
#include "VarArray.h"

typedef struct VAR_ARRAY VarArray{
    int capacity;
    int currIndex; // next empty index
    int *items;
}

static void validateArray( VarArray *list ) {
    assert(list != NULL);
    if(list != NULL){
        assert(list->capacity > 0 && list->currIndex < list->capacity);
        assert(list->items != NULL);
    }
}

// return a new VarArray with given initial capacity
VarArray *newVarArray( int capacity ){
    assert(capacity > 0);
    VarArray *list = (VarArray*)malloc(sizeof(VarArray));
    assert(list != NULL);

    if(list != NULL){
        list->capacity = capacity;
        list->currIndex = 0;
        list->items = (int*)malloc(sizeof(int)*capacity);

        // post conditions
        assert(list->items != NULL && size(list) == capacity);
        validateArray(list); 
    }

    return list;
}

// delete the given VarArray
void deleteVarArray( VarArray *list ){
    free(list->items);
    free(list);
}

// insert data at the end of the VarArray,
//expanding the array if necessary
void add( VarArray *list, int data ){
    assert(data != NULL); // pre conditions

    if(data != NULL){
        validateArray(list);
        list->items[list->currIndex++] = data; // add the data

        // expand
        if(list->currIndex+1 == list->capacity){
            list->capacity *= 2;
            int *newItems = (int*)malloc(sizeof(int)*list->capacity); // new list
            
            // copy from old list
            for(int i=0; i<list->currIndex; i++){
                newItems[i] = list->items[i];
            }
            free(list->items); // free the old list
            list->items = newItems; // assign the new list
            validateArray(list); // post condition
        }
    }

    assert(contains(list, data) == true); // post condition
}

// remove item at given index from the VarArray
// you **must** maintain contiguous data
void remove( VarArray *list, int index ){
    // pre conditions
    assert(index > -1 && index < list->currIndex);
    int item = get(list, index);
    assert(item != NULL);
    validateArray(list);

    if(item != NULL){
        // shift all the item(s) left if there's any
        for(int i=index; i<list->currIndex; i++){
            list->items[i] = list->items[i+1];
        }
        list->currIndex--;
        validateArray(list); // post condition
    }
}

// variable declarations (if any)

// prototypes (if any)
void removeTest(VarArray *list, int index){
    printf("Removing item at index %d\n", index);
    remove(list, index);
}

int main(void){
    int dataSize = 5;
    int data[] = { 1, 2, 3, 4, 5 };
    int aSize = 4;
    printf("Creating a new var array with size %d\n", aSize);
    VarArray *arr = newVarArray(aSize);
    
    // insertion and expansion test
    for(int i=0; i<dataSize; i++){
        printf("Adding %d to position %d\n", data[i], arr->currIndex);
        add(arr, data[i]);
    }

    if(size(arr) == aSize*2){
        printf("Expanded successfully!\n");
    }
    else{
        printf("Failed to expand the array list\n");
    }

    printf("Current size of the array list %d\n", size(arr));

    // removation test
    removeTest(arr, 2);
    removeTest(arr, 3);
    if(arr->currIndex = 3){
        printf("Success in keeping the list contigous\n");
    }
    else{
        printf("Failed to keep the list contigous\n");
    }

    // clearing test
    deleteVarArray(arr);
    if(arr->list == NULL){
        printf("Success in deleting the array list\n");
    }
    else{
        printf("Failed to delete the array list\n");
    }

    return 0;
}