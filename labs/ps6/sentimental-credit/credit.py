def luhns_algo(s):
    if len(s) < 13 or len(s) > 16:
        return False

    # multiply every other number by two
    multisums = "".join([str(int(n)*2) for n in s[::2]])

    # sum every other number
    summulti = sum([int(n) for n in multisums])

    # sum numbers not multiplied
    normalsums = sum([int(n) for n in s[1::2]])

    # return true if the sums return a number with last digit zero
    return str(summulti + normalsums)[-1] == "0"


def validate(s):
    try:
        num = int(s)
        first_two = s[:2]
        card_type = ""
        luhn_check = luhns_algo(s)
        # All visa's start with 4
        if "4" == first_two[0] and luhn_check:
            card_type = "VISA"
        # 34 or 37 = amex
        elif first_two in ["34", "37"]:
            card_type = "AMEX\n"
        # 51 - 55 = mastercard
        elif first_two in ["51", "52", "53", "54", "55"] and luhn_check:
            card_type = "MASTERCARD\n"
        else:
            card_type = "INVALID\n"

        return (card_type, num)
    except ValueError:
        print("Card must only be numeric..")
        return False


def main():
    # get user input
    get_input = input("Number: ")
    # pass the value returned from function call into a variable
    valid = validate(get_input)
    # check if valid returned false, if not, print the card_type
    if (valid != False):
        print(valid[0])


# safe guard to not accidentally invoke other scripts. not really needed here, but just good practice.
if __name__ == "__main__":
    main()