#include <stdio.h>


int main(void)
{
    unsigned short colour = 0;

    unsigned char r, g, b;

    r = 21;
    g = 0;
    b = 25;

    colour = colour | b << 10;
    colour = colour | g << 5;
    colour = colour | r;

    //colour = r | g << 5 | b << 10;

    printf("The colour is %x\n", colour);

    // if you're bored on Sunday, you can
    // try to write a function that will
    // print out colour in binary.

    return 0;

}
