
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
    int full_length = (height * 2) + 2;
    //for every row
    for (int i = 0; i < height; i++)
    {
        printf("\n");
        //for every column
        for (int j = 0; j < full_length; j++)
        {
            //if j is less than the height minus the row index, print space
            if (j < height - i - 1)
            {
                printf(" ");
            }
            //if j is less than the height plus the row index, plus two, print nothing.
            //This ensures that only the necessary amount of hashes will be printed after the space between steps
            else if (j > height + i + 2)
            {
                printf("");
            }
            //height and height + 2 are the literal spaces between the steps
            else if (j == height || j == height + 1)
            {
                printf(" ");
            }
            //if all conditions to print spaces are false, print a hash instead
            else
            {
                printf("#");
            }
        }
    }
    printf("\n");
}