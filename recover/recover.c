#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//define BYTE
typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;


int main(int argc, char *argv[])
{
    //check usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //check raw file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 2;
    }

    //the number of image
    int counter = 0;
    //define pointer of image
    FILE *image;
    //define a string for image name
    char file_name[8];
    //create a array to store 512-byte block
    BYTE buffer[BLOCK_SIZE];

    //repeat until end of the card
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE) //read 512 bytes into a buffer
    {
        //if start of new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if not the first JPEG
            if (counter != 0)
            {
                fclose(image);
            }

            //write the file
            sprintf(file_name, "%03i.jpg", counter);//creat a new name to file_name
            image = fopen(file_name, "w");//create and open a new image
            fwrite(&buffer, 1, BLOCK_SIZE, image);//write buffer to the image
            counter++;
        }

        //if already found new JPEG, keep writing in it
        else
        {
            if (counter != 0)
            {
                fwrite(buffer, 1, BLOCK_SIZE, image);
            }
        }
    }
    
    //close any remaining files
    fclose(image);
    fclose(raw_file);
    return 0;
}