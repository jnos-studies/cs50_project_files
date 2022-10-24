#include <stdio.h>
#include <cs50.h>

//declare a pyramid function
void make_pyramid(int height);

int main(void)
{
    int user_choice;
    //if the user_choice is less than 1 or greater than eight ask the question again.
    do
    {
        user_choice = get_int("Height Please: ");
    }
    while (user_choice < 1 || user_choice > 8);
    make_pyramid(user_choice);
}

void make_pyramid(int height) //0(n^2)
{
    //create a value for the number of spaces one needs
    int spaces = height - 2;
    //for every row
    for (int i = 0; i < height; i++)
    {
        printf("\n");
        //for every column
        for (int j = 0; j < height; j++)
        {
            //if j is less than number of required spaces print space, otherwise print '#'
            if (j <= spaces)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        //decrease the number of required spaces for each row iteration
        spaces--;
    }
    printf("\n");
}