main: ObjectManager.h ObjectManager.o main.c
	clang++ -Wall main.c ObjectManager.o -o main

ObjectManager.o: ObjectManager.h ObjectManager.c
	clang++ -Wall -c ObjectManager.c -o ObjectManager.o

clean: 
	rm -f ObjectManager.o main