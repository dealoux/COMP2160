#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_LENGTH 11

int main( void )
{
    char input[INPUT_LENGTH];                    
    char dateData[3][4]; 
    char *s = "-";
    char *token;
    int decimal = 0;
    int result = 0;
    int rem = 0;
    int hex = 0;

    while ( fgets( input, INPUT_LENGTH, stdin ) != NULL )
    {
        //printf("%s ", input);
        token = strtok(input, s);
        int index = 0;
        
        while( token != NULL )
        {
            strcpy(dateData[index++], token);
            token = strtok(NULL, s);
        }

        decimal = decimal | atoi(dateData[0]) << 24;
        decimal = decimal | atoi(dateData[1]) << 16;
        decimal = decimal | atoi(dateData[2]);
        printf("%d", decimal);

        // result = decimal;
        // int count = 0;
        // while(result - rem != 0){
        //     result = result/16;
        //     rem = result%16;
        //     hex = hex | rem << 4^count;
        //     count++;
        // }

        // printf("%s-%s-%s %d %d",dateData[0], dateData[1], dateData[2], decimal, hex);
    }
    
    return 0;           
}
