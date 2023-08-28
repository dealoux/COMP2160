typedef struct VAR_ARRAY VarArray;
typedef enum {false, true} boolean;

// return a new VarArray with given initial capacity
VarArray *newVarArray( int capacity );

// delete the given VarArray
void deleteVarArray( VarArray *list );

// return # of elements in the VarArray
// NOTE: You will not implement this, but
// it's available for testing purposes
int size( VarArray *list );

// insert data at the end of the VarArray,
//expanding the array if necessary
void add( VarArray *list, int data );

// remove item at given index from the VarArray
// you **must** maintain contiguous data
void remove( VarArray *list, int index );

// return item at given index in the VarArray
// NOTE: You will not implement this, but
// it's available for testing purposes
int get( VarArray *list, int index );

// return true if data is in the VarArray
// NOTE: You will not implement this, but
// it's available for testing purposes
boolean contains( VarArray *list, int data );








1  #include <string.h>

typedef struct NODE node;

typedef struct LINKEDLIST list;

// constructor function
list *makeList(void);

// constructor function
node *makeNode(char *);

// mutator (add item to list)
void addItem(list *, char *);

// destructor function
void destroyList(list *);

// destructor function
void destroyNode(node *);

10 // invariant for node
11 void checkNode(node *n) {
12       // assertions
13 }

14 // invariant for list
15 void checkList(list *l) {
16       // assertions
17 }