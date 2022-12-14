#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }
}

int compute_score(string word)
{
    char *LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int score = 0;
    // TODO: Compute and return score for string
    int len_l = strlen(LETTERS);
    int len_w = strlen(word);
    //My 0(n^2) algorythm used to solve this problem. Bubble check each and every Letter and match it sequentially with the player's word choice
    for (int i = 0; i < len_l; i++)
    {
        for (int w = 0; w < len_w; w++)
        {
            if (LETTERS[i] == toupper(word[w]))
            {
                score += POINTS[i];
            }
        }
    }
    return score;
}
