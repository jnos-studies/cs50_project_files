#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

float L(char *text);
int main(void)
{
    char *text = get_string("\nText: ");
    float result = L(text);

    // Check the result of the Coleman Liau function
    if (result >= 16.0)
    {
        printf("Grade 16+\n");
    }
    else if (result < 1.0)
    {
        printf("Before Grade 1\n");
    }
    else if (result < 2.5 && result > 2.0)
    {
        printf("Grade %d\n", 2);
    }
    // Print the grade
    else
    {
        printf("Grade %d\n", (int)round(result));
    }
    return 0;
}

float L(char *text)
{
    int words = 0;
    int spaces = 0;
    int sentences = 0;
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // Check if character is a space.
        if (isspace(text[i]))
        {
            spaces++;
        }
        // Check for puncuation
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
        // Check if character is alphanumeric
        else if (isalnum(text[i]))
        {
            letters++;
        }
    }
    words = spaces + 1;
    // Calculate the Grade and remember to typecast the integers as floats.
    float coleman_liau = 0.0588 * (100.0 * ((float)letters / (float)words)) - 0.296 * (100.0 * ((float)sentences /
                         (float)words)) - 15.8;


    return coleman_liau;
}