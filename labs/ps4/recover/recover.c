#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;

    if (argv[1] == NULL)
    {
        printf("\n./recover filename.raw");
        return 1;
    }
    //initializing variables
    BYTE block_buffer[512];
    FILE *card = fopen(argv[1], "r");
    int found_matches = 0;
    FILE *outptr = NULL;
    char *filename = malloc(sizeof(char) * 7);

    while (fread(block_buffer, 1, 512, card) == 512)
    {
        if (block_buffer[0] == 0xff && block_buffer[1] == 0xd8 && block_buffer[2] == 0xff && (block_buffer[3] & 0xf0) == 0xe0)
        {
            //if not the first jpeg close the previous
            if (!(found_matches == 0))
            {
                fclose(outptr);
            }
            //initialize me file
            sprintf(filename, "%03i.jpg", found_matches);
            outptr = fopen(filename, "w");
            found_matches++;
        }
        //if jpeg has been found, write that file!
        if (!(found_matches == 0))
        {
            fwrite(block_buffer, 512, 1, outptr);
        }
    }
    free(filename);
    //printf("\nComplete!, found %d jpgs.\n", found_matches);


    fclose(card);
    fclose(outptr);

    return 0;
}
