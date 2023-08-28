//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 4
// 
// REMARKS: This module is the test suite for the Object Manager
//
//-----------------------------------------
 
#include <stdio.h>
#include <assert.h>
#include "ObjectManager.h"

static void test(){
  char *ptr;
  int i;
  Ref id1,id2,id3;

  // Normal case testing
  initPool();

  fprintf(stdout, "Testing normal case\n");
  
  id1= insertObject(100);
  fprintf(stdout, "id1 = %lu\n", id1);
  ptr = (char*)retrieveObject(id1);
  for (i = 0; i < 100; i++)
    ptr[i] = (char)(i%26 + 'A');
  
  id2 = insertObject(4000);
  id3 = insertObject(20200);
  fprintf(stdout, "The Pool before dropping 3\n");
  dumpPool();
  dropReference(id3);
  addReference(id2);
  id3 = insertObject(10);
  fprintf(stdout, "The pool after dropping 3\n");
  dumpPool();
  fprintf(stdout, "Testing compact ability\n");
  dropReference(id2);
  dropReference(id2);
  id2 = insertObject(MEMORY_SIZE - 4000 - 100 - 10);
  
  ptr = (char*)retrieveObject(id1);
  for (i = 0; i < 100; i++)
    fprintf(stdout,"%c",ptr[i]);
  fprintf(stdout,"\n");
  
  fprintf(stdout, "The pool after compact\n");
  dumpPool();
  destroyPool(); 
  fprintf(stdout, "Cleard the pool\n");
  fprintf(stdout,"---\n");

// Edge case testing
#ifndef NDEBUG
  fprintf(stdout, "\nTesting edge case, the program will terminte by line 61\n");
  id1 = insertObject(50);
  id2 = insertObject(MEMORY_SIZE);
  dropReference(id2);
  addReference(id2);
  addReference(id1);
  dumpPool();
  retrieveObject(id2);
#endif  
}

int main(void)
{
  test();
  fprintf(stdout,"The program terminated normally\n");
  return 0;
}

