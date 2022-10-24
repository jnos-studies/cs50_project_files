#include <stdio.h>
#include <cs50.h>

//Main dat void!
int main(void)
{
    //Ask the user for their name and recieve the input value that the user provides.
    string name = get_string("What is your name?\n");
    //printf the final message to user, welcoming them to the program before termination.
    printf("Well hello there %s!!\n", name);
}