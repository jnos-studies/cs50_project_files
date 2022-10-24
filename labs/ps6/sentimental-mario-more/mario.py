def mario():
    # try the following and then except any ValueErrors that could occur due to type issues, recall user for input via recursion.
    try:
        get_height = int(input("Height: ")) + 1
        if get_height - 1 == -1 or get_height - 1 == 0:
            mario()
        elif get_height - 1 == 9:
            mario()
        # define the block character and the space character for readability
        block_char = "#"
        space = " "

        # loop over the blocks and print out for each loop.
        for blocks in range(get_height):
            if blocks != 0:
                print(space*(get_height - blocks - 1) + block_char*blocks + "  " + block_char*blocks)
    except ValueError:
        # when a value error occurs, recursively call the mario function
        mario()


# call the function
mario()