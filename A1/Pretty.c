//-----------------------------------------
// NAME: Tom Le 
// STUDENT NUMBER: 7871324
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Mehdi Niknam
// ASSIGNMENT: assignment 1, QUESTION: question 1
// 
// REMARKS: This program reads in lines of text and combines/separates them as appropriated
//
//-----------------------------------------

#include <stdio.h>
#include <string.h>

#define LINE_SIZE 50

int main(void){
    char inputLine[LINE_SIZE];
    int position = 1;

    while(fgets(inputLine, LINE_SIZE, stdin) != NULL)
    {
        inputLine[strcspn(inputLine, "\n")] = 0; // omit the trailing \n in the result from fgets()

        if(strcmp(inputLine, "") == 0){
            if(position > 1){
                printf("\n");
            }

            printf("\n");
            position = 1;
        }

        else{
            if((position + strlen(inputLine)-1) > LINE_SIZE){
                printf("\n");
                position = 1;
            }
            printf("%s", inputLine);

            position += strlen(inputLine);
            if(position <= LINE_SIZE)
            { // add a blank after the current word
                printf(" ");
                position++;
            }
        }
    }

    fclose(stdin);
    return 0;
}
