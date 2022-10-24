#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
char *encrypt(string key, char *phrase);

int main(int argc, string argv[])
{
    if (argc > 2 || argc < 2 || argv[1] == NULL)
    {
        printf("\nUsage: ./substitution key\n");
        return 1;
    }
    string key = argv[1];
    int key_len = strlen(key);
    //Enforce a length check on the key as well as length of arguments. Remind user how to use program.
    for (int k = 0; k < key_len; k++)
    {
        for (int d = key_len; d > k; d--)
        {
            if (key[d] == key[k])
            {
                printf("Error: Key cannot contain duplicates\n");
                return 1;
            }
        }
        if (!isalpha(key[k]))
        {
            printf("Error: Key cannot contain non-alphabetic characters\n");
            return 1;
        }
    }

    if (key_len > 26 || key_len < 26)
    {
        printf("\nUsage: ./substitution key\n");
        return 1;
    }

    char *user_phrase = get_string("plaintext: ");


    //check that every item in key is alphanumeric then encrypt phrase with the key
    for (int i = 0; i < key_len; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Error: the key must only contain alphanumeric characters.");
            printf("\nUsage: ./substitution key\n");
            return 1;
        }
    }
    char *encryption = encrypt(key, user_phrase);
    printf("ciphertext: %s\n", encryption);
    //free the allocated memory because it is out of the stack.
    free(encryption);

    return 0;
}


char *encrypt(string key, char *user_phrase)
{
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int phrase_len = strlen(user_phrase);
    int i, j;

    //Allocate memory to assign values to an array
    char *result = malloc(phrase_len + 1);

    //0(n^2) Big O algorithm that checks each character of the user phrase against a position in the alphabet
    //Then adds the value of a given position to the index of the allocated array.
    for (i = 0; i < phrase_len; i++)
    {
        for (j = 0; j < 26; j++)
        {
            char upper = toupper(alphabet[j]);
            //if the character is lowercase
            if (user_phrase[i] == alphabet[j])
            {
                result[i] = tolower(key[j]);
            }
            //if the character is upper case
            else if (user_phrase[i] == upper)
            {
                result[i] = toupper(key[j]);
            }
            //if the character is not alphabetic or is a space
            else if (!isalpha(user_phrase[i]) || isspace(user_phrase[i]))
            {
                result[i] = user_phrase[i];
            }
        }
    }
    return result;
}