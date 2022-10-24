import re
import math


def L(string):
    # find all matches of alphabetic characters constituting a word
    words = re.findall("[a-zA-Z]+", string)
    # define a variable for the length of words
    word_l = len(words)
    # join words together to get number of letters
    letters = len("".join(words))
    # find all sentence ending characters in the string and return i in a list. get length of list for number of sentences
    sentences = len([i for i in string if i == "." or i == "?" or i == "!"])

    # calculating the coleman laiu index
    coleman_liau = 0.0588 * (letters / word_l * 100) - 0.296 * (sentences / word_l * 100) - 15.8

    # return the ceiling value for the calculation
    return coleman_liau


def main():
    try:
        # get the user text
        usr_text = str(input("Write something: "))
        result = L(usr_text)

        # if else for the result of the coleman liau index
        if math.ceil(result) >= 16:
            print("Grade 16+")
        elif math.ceil(result) < 1:
            print("Before Grade 1")
        elif result < 2.5 and result > 2.0:
            print("Grade 2")
        else:
            print(f"Grade {math.ceil(result)}")
    # if an error occurs, just recursively call this function until user inputs valid
    except:
        main()


if __name__ == "__main__":
    main()