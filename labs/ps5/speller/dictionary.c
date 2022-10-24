// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// buckets in hash = 2^16
const unsigned int N = 262144;

// Hash table
node *table[N];

//total words
int total = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int length = strlen(word);
    char copy[length + 1];
    //add the ntc character after allocating space
    copy[length] = '\0';

    for (int i = 0; i < length; i++)
    {
        //convert any uppercase letters to lower making it case-insensitive
        copy[i] = tolower(word[i]);
    }

    unsigned int word_hash = hash(copy);


    node *temp = table[word_hash];
    if (temp == NULL)
    {
        return false;
    }
    while (temp != NULL)
    {
        if (strcasecmp(temp->word, copy) == 0)
        {
            return true;
        }
        temp = temp->next;
    }
    return false; // if not found copy until end

}

// Hashes word to a number with references I needed. the implementation of the hash is similar to the one used in one of the links, but I played around with the hash number and figured out how the bitwise operator works. Making it my own.
// https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
// https://gist.github.com/srushtikanade/b0612f537a15e1265f624dd2d946a917
unsigned int hash(const char *word)
{
    unsigned long hash = 6381;
    int character = *word;
    while (character == *word++)
    {
        //recalculating the hash move left, increases number by 32 times, adding the unicode character numbers which makes it unique.
        //doesn't really matter what numbers i add, the unique set of characters will create the same hash regardless
        hash = ((hash << 5) + hash) + character;
    }
    //hash modulus the Number of buckets in the hash table
    //increasing the bucket size makes it a little faster but uses more memory.
    return hash % N;

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {

        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        else
        {
            strcpy(n->word, word);
            n->next = NULL;
            unsigned int index = hash(word);

            //link the n as the new head.
            n->next = table[index];
            table[index] = n;
            total += 1;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //loop through every linked list
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    return true;
}
