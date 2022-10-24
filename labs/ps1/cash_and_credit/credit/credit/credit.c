#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//declaring functions.
int get_int_length(long n);
const char *card_type(long n, int length);
bool check_sum(long n, int length);

int main(void)
{
    //Variables
    long get_num;
    int len;
    const char *c_type;
    bool check_s;

    //Populate those variables.
    get_num = get_long("Number: ");
    len = get_int_length(get_num);
    c_type = card_type(get_num, len);
    check_s = check_sum(get_num, len);

    //If the check_s (Luhn's Algorythm returns valid) and the card type (c_type) hasn't returned 0,
    //printf the card type.
    if (check_s == true && c_type != 0)
    {
        printf("%s\n", c_type);
    }
    else
    {
        printf("%s\n", "INVALID");
    }
}

//Function to get the length of the card number.
int get_int_length(long n)
{
    int length = 1;
    while (n /= 10)
    {
        length++;
    }
    return length;
}

//Determine the card's type, out of three types mentioned
const char *card_type(long n, int length)
{
    //MASTERCARD - MasterCard numbers start with 51, 52, 53, 54, or 55
    char *master_card = "MASTERCARD";
    //AMEX - All American Express numbers start with 34 or 37
    char *amex = "AMEX";
    //VISA -  all Visa numbers start with 4
    char *visa = "VISA";

    //return the first two digits of the number
    int first_two = n / pow(10, length - 2);

    //Logic to test the first two numbers of the card number to determine card type.
    if (first_two / 10 == 4)
    {
        return visa;
    }
    else if (first_two == 34 || first_two == 37)
    {
        return amex;
    }
    else if (first_two == 51 || first_two == 52 || first_two == 53 || first_two == 54 || first_two == 55)
    {
        return master_card;
    }
    //return zero even if the given card number does not satisfy these if statements.
    return 0;
}

//implementing Luhn's Algorithym
bool check_sum(long n, int length)
{
    if (length < 13 || length > 16)
    {
        return false;
    }

    //Sum variables
    int mult_sum = 0;
    int nrm_sum = 0;
    int result_sum = 0;

    for (int i = 0; i < length; i++)
    {
        int num = n % 10;
        if (i % 2 != 0)
        {
            //If the multiplied digit number is larger than 9, multiply the first and last digit of multiplied digit.
            if ((num * 2) > 9)
            {
                mult_sum += (num * 2) % 10;
                mult_sum += (int)(num * 2) / 10;
            }
            //otherwise add to multiplied sums normally
            else
            {
                mult_sum += num * 2;
            }
        }
        //sum every number not multiplied by 2
        else
        {
            nrm_sum += num;
        }
        n = n / 10;
    }

    //add multiplied by 2 sums and nrm_sum together and check if the last digit == 0 to see if valid or not.
    result_sum += mult_sum + nrm_sum;

    if (result_sum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}