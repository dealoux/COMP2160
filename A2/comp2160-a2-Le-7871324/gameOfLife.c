//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 2
// 
// REMARKS: This program is a implement of Conway's Game of Life, while also applying the principles of Design by Contract
//
//-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INPUT_LENGTH 80
#define UNIVERSE_SIZE 60
#define MAX_GEN 251

#ifdef NDEBUG
  #define PRINT_VALUE 10
#else
  #define PRINT_VALUE MAX_GEN
#endif

/* Structures */
typedef struct GENERATION
{
   int universe[UNIVERSE_SIZE][UNIVERSE_SIZE]; // a 2D array to store a universe
} generation;

typedef struct GAME
{
    // total number of rows and cols for all the generations
    int rows; 
    int cols;

    int currGen; // index of the current most recent generation, add 1 for the total length of the generation array
    generation gens[MAX_GEN]; // an array to store all the generations of the game
} game;

/* Game variables */
game *g; // pointer to the current game
int newGame; // flag for initialization, act as a boolean

/* Array traversal variables */
char input[INPUT_LENGTH]; // caching string inputs from the file
int currRow = 0; // row index for initializing the universe gen 0, increasing along with the input lines

FILE *o; // pointer to the output file

/* Function prototypes */
void printGen(generation gen, int num);
void printGame();
void runGame();
void inputHandler();

// This function handles the output file and calls inputHandler() to handle the given input
int main(void){
    g = malloc(sizeof(game));
    assert(g != NULL);
    o = fopen("A2out.txt", "w");
    assert(o != NULL);

    if(o != NULL){
        inputHandler();
        fclose(o);
    }
    
    printf("The program terminated normally\n");
    return 0;
} // end of main()

// This function free the current game and allocate a new game game to the pointer, called by inputHandler()
void allocateGame(){
    free(g); // free the current game if exist
    g = malloc(sizeof(game));
    assert(g != NULL);
    g->rows = 0;
    g->cols = 0;
    g->currGen = 0;
} // end of allocateGame();

/* Game logic functions */
// invariant for checking a generation, called by validateGame()
void validateGen(generation gen){
    assert(gen.universe != NULL);

    for(int r=0; r < g->rows; r++){
        for(int c=0; c < g->cols; c++){
            assert(gen.universe[r][c] == 0 || gen.universe[r][c] == 1);
        }
    }
} // end of validateGen()

// invariant for the game, validate after each generation is added
// called by inputHandler once and runGame() every loop
void validateGame(){
    assert(g != NULL);
    // assert(g->rows < UNIVERSE_SIZE && g->rows > 0);
    // assert(g->cols < UNIVERSE_SIZE && g->cols > 0);
    assert(g->gens != NULL);
    assert(g->currGen >= 0);
    
    for(int i=0; i<g->currGen; i++){
        validateGen(g->gens[i]);
    }
} // end of validateGame()

// This function contains all the logic for handling the input, and playing the game
void inputHandler(){
    // precondition
    assert(stdin != NULL);

    while ( fgets( input, INPUT_LENGTH, stdin ) != NULL )
    {
        // checks for new game
        if(input[0] == '*'){ // new game found
            fprintf(o, "%s", input); // printing the new game title
            allocateGame(); 
            currRow = 0; // reset row index to 0 each game
            newGame = 1; // set to true for initializing
            continue;
        }
        // else start initializing, filling and playing the game
        else{
            // initializing
            if(newGame){
                // obtaing the row and column numbers
                g->rows = atoi(strtok(input, " "));
                g->cols = atoi(strtok(NULL, " "));
                // checks if they are within range
                assert(g->rows < UNIVERSE_SIZE && g->rows > 0);
                assert(g->cols < UNIVERSE_SIZE && g->cols > 0);

                newGame = 0; // game initialization done, setting the flag to false
            }
            // start filling & playing
            else if(currRow <= g->rows){
                // filling
                for(int c=0; c < g->cols; c++){
                    g->gens[0].universe[currRow][c] = (input[c] == 'X') ? 1 : 0;
                }
                
                // done filling, start playing
                if(++currRow == g->rows){
                    // validate and print the initial generation 0
                    g->currGen = 0; 
                    validateGame();
                    printGen(g->gens[g->currGen], g->currGen);

                    // run and print the game
                    runGame();
                    printGame();
                }
            } // end of game initialization (newGame) if else
        } // end of a new game is detected (input starts with *) if else
    } // end of while input is not EOF
} // end of inputHandle()

// This function checks and counts all the alive neighbours of the given cell, called by compareGen()
// r and c are the index of the given cell
int countNeighbours(generation gen, int r, int c){
    int result = 0; 
    // Edge checking variables, act as booleans
    int notTopEdge = (r > 0) ? 1 : 0;
    int notBottomEdge = (r+1 < g->rows) ? 1 : 0;
    int notLeftEdge = (c > 0) ? 1 : 0;
    int notRightEdge = (c+1 < g->cols) ? 1 : 0;

    // left neighbour
    if(notLeftEdge)
        result += gen.universe[r][c-1];
    
    // right neighbour
    if(notRightEdge)
        result += gen.universe[r][c+1];

    // above neighbours
    if(notTopEdge){
        result += gen.universe[r-1][c]; // direct top neighbour

        // top left neighbour
        if(notLeftEdge)
            result += gen.universe[r-1][c-1];

        // top right neighbour
        if(notRightEdge)
            result += gen.universe[r-1][c+1];
    }

    // below neighbours
    if(notBottomEdge){
        result += gen.universe[r+1][c]; // direct bottom neighbour

        // bottom left neighbour
        if(notLeftEdge)
            result += gen.universe[r+1][c-1]; 

        // bottom right neighbour
        if(notRightEdge)
            result += gen.universe[r+1][c+1];
    }
        
    return result;
} // end of countNeighbours()

// This function compares the two given generation universes by looping through them pair by pair of cells, called by cycleFound()
// if a pair of cell does not equal to each other, the loop will break and the function will then return 0
// otherwise after the loop function return 1 by default, indicating they are equal
int compareGen(generation gen1, generation gen2){
    // preconditions
    assert(gen1.universe != NULL);
    assert(gen2.universe != NULL);

    int result = 1; // default, equal

    for(int r=0; r < g->rows; r++){
        for(int c=0; c < g->cols; c++){
            if(gen1.universe[r][c] != gen2.universe[r][c]){
                result = 0; // false, not equal
                break;
            } 
        }
    }

    return result;
} // end of compareGen()

// This function loops through every generations of the game and call compareGen() on the most recently added current generation to each of the previous generation(s)
// a cycle is found if the result of a single comparison is true, a message will be printed and the loop will be terminated, the function will then return 1
// otherwise after the loop the function will return the default 0 indicating no cycle is found
int cycleFound(){
    int result = 0; // default, no cycle

    for(int i=0; i < g->currGen; i++){
        if( compareGen(g->gens[i], g->gens[g->currGen]) ){
            fprintf(o, "Found a cycle between generation %d and generation %d\n", i, g->currGen);
            result = 1; // cycle found
            break;
        }
    }

    return result;
} // end of cycleFound()

// This function runs the game until a cycle is found or until generation 250, which ever comes first, called by inputHandler()
// during each run it will loop through every cell of a generation, count its alive neighbours and decide whether or not it should be dead or alive next generation base on the current status
// the game will also be validated after each run
void runGame(){
    int neighboursCount; // caching the neighbours count result of each cell

    // loop from gen 1 to gen 250 excluding gen 0
    for(int i=1; i<MAX_GEN; i++){
        neighboursCount = 0;

        // loop through every cell in the generation
        for(int r=0; r < g->rows; r++){
            for(int c=0; c < g->cols; c++){
                neighboursCount = countNeighbours(g->gens[g->currGen], r, c);

                // if the current cell is dead
                if(g->gens[g->currGen].universe[r][c] == 0){
                    g->gens[g->currGen + 1].universe[r][c] = (neighboursCount == 3) ? 1 : 0;
                }
                // else it's alive
                else{
                    assert(g->gens[g->currGen].universe[r][c] == 1); // making sure it's 1
                    g->gens[g->currGen + 1].universe[r][c] = (neighboursCount < 2 || neighboursCount > 3) ? 0 : 1;
                } // end of else
            }
        } // end of nested for loop on universe 2d array

        g->currGen++;
        validateGame(); // post condition

        // also break the game loop if a cycle is found
        if(cycleFound())
            break;
    } // end of for loop on generations array
} // end of runGame()


/* Printing functions */

// This function is called by printGen(), printing the top and bottom border with the given size of a generation
void border(){
    fprintf(o, "+");

    for(int i=0; i<g->cols; i++){
        fprintf(o, "-");
    }

    fprintf(o, "+\n");
}

// This function checks whether the given cell is alive or dead and print it accordingly, called by printGen()
void printCell(int cell){
    if(cell == 0)
        fprintf(o, ".");
    else{
        assert(cell == 1); // precondition
        fprintf(o, "*");
    }
} // end of printCell()

// This function prints the given genenation of a game, called by printGame()
void printGen(generation gen, int num){
    fprintf(o, "Generation %d:\n", num); // gen number

    border(); // top border

    // loop through every cell in the generation
    for(int r=0; r < g->rows; r++){
        fprintf(o, "|"); // left border

        for(int c=0; c < g->cols; c++){
            printCell(gen.universe[r][c]);
        }

        fprintf(o, "|\n"); // right border
    }

    border(); // bottom border
} // end of printGen()

// This function prints all the generation(s) from start to end inclusive
void printGameSection(int start, int end){
    for(int i=start; i <= end; i++){
        printGen(g->gens[i], i);
    }
} // end of printGameSection()

// This function calculates the starting point, base on the PRINT_VALUE and the game size, then calls printGameSection() to print from it to the current gen
void printGame(){
    int start = (g->currGen+1 > PRINT_VALUE) ? g->currGen+1 - PRINT_VALUE : 0; // prevent negative
    assert(start >= 0 && start <= g->currGen); 

    printGameSection(start, g->currGen);

    fprintf(o, "\n");
} // end of printGame()