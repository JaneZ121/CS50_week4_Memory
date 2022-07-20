#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            LONG sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            LONG sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            LONG sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            sepiaRed = (sepiaRed > 255) ? 255 : sepiaRed;
            sepiaGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            sepiaBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width + 1) / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //copy image
    RGBTRIPLE(*copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    //RGBTRIPLE copy[height][width];
    if (copy == NULL)
    {
        printf("Not enough memory to store image.\n");
        return;
    }
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w].rgbtRed = image[h][w].rgbtRed;
            copy[h][w].rgbtGreen = image[h][w].rgbtGreen;
            copy[h][w].rgbtBlue = image[h][w].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            LONG sumRed = 0;
            LONG sumGreen = 0;
            LONG sumBlue = 0;
            float count = 0;
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if ((i + r) >= 0 && (i + r) < height && (j + c) >= 0 && (j + c) < width)
                    {
                        sumRed += copy[i + r][j + c].rgbtRed;
                        sumGreen += copy[i + r][j + c].rgbtGreen;
                        sumBlue += copy[i + r][j + c].rgbtBlue;
                        count++;
                    }
                }
            }
            image[i][j].rgbtRed = round(sumRed / count);
            image[i][j].rgbtGreen = round(sumGreen / count);
            image[i][j].rgbtBlue = round(sumBlue / count);
        }
    }
    free(copy);
    return;
}
