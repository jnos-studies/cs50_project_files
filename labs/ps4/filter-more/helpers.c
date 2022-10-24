#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//I have commented on areas I had to research to figure out the problem.

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // References
            // https://www.reddit.com/r/cs50/comments/rdal3s/help_for_filter_grayscale_pleasee/

            // Searched that the reason my greyscale was not filtering correctly was because I was
            // not droping the decimal place by rounding it to the nearest integer
            // not realising that in C, integers divided by integers will be integers.
            // I was also allocating memory incorrectly, so I used a double type instead of a BYTE and
            // put it on the stack.
            average = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            image[i][j].rgbtRed = image[i][j].rgbtBlue = image[i][j].rgbtGreen = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // I was extending my j variable out of the array limit of the array, I was also
    // swapping the entire width of the pixel, I had to divid the width by two and then
    // account for the middle pixel by using width - 1 when accessing the width in a 2 dimensional array
    // I was also making my code too complicated by declaring multiple variables
    // and storing the 8bit rbgt values into a 24bit RGBTRIPLE data type
    // and not using a temporary variable instead to handle one single pixel.

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_img[height][width];

    // Make a copy of the image array and store it in the stack
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_img[i][j] = image[i][j];
        }
    }

    //Loop through the dimensions of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Top part of image
            if (i == 0)
            {
                // Left corner edge case
                if (j == 0)
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i + 1][j].rgbtRed +
                                                    temp_img[i][j + 1].rgbtRed +
                                                    temp_img[i + 1][j + 1].rgbtRed) / 4.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i + 1][j].rgbtGreen +
                                                      temp_img[i][j + 1].rgbtGreen +
                                                      temp_img[i + 1][j + 1].rgbtGreen) / 4.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i + 1][j].rgbtBlue +
                                                     temp_img[i][j + 1].rgbtBlue +
                                                     temp_img[i + 1][j + 1].rgbtBlue) / 4.0);
                }
                // Right corner edge case
                else if (j == width - 1)
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i + 1][j].rgbtRed +
                                                    temp_img[i][j - 1].rgbtRed +
                                                    temp_img[i + 1][j - 1].rgbtRed) / 4.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i + 1][j].rgbtGreen +
                                                      temp_img[i][j - 1].rgbtGreen +
                                                      temp_img[i + 1][j - 1].rgbtGreen) / 4.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i + 1][j].rgbtBlue +
                                                     temp_img[i][j - 1].rgbtBlue +
                                                     temp_img[i + 1][j - 1].rgbtBlue) / 4.0);
                }
                // Handle everything else that is not a corner.
                else
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i + 1][j].rgbtRed +
                                                    temp_img[i][j - 1].rgbtRed +
                                                    temp_img[i + 1][j - 1].rgbtRed +
                                                    temp_img[i][j + 1].rgbtRed +
                                                    temp_img[i + 1][j + 1].rgbtRed) / 6.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i + 1][j].rgbtGreen +
                                                      temp_img[i][j - 1].rgbtGreen +
                                                      temp_img[i + 1][j - 1].rgbtGreen +
                                                      temp_img[i][j + 1].rgbtGreen +
                                                      temp_img[i + 1][j + 1].rgbtGreen) / 6.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i + 1][j].rgbtBlue +
                                                     temp_img[i][j - 1].rgbtBlue +
                                                     temp_img[i + 1][j - 1].rgbtBlue +
                                                     temp_img[i][j + 1].rgbtBlue +
                                                     temp_img[i + 1][j + 1].rgbtBlue) / 6.0);
                }
            }
            // If not at the top of the image and one is less than the overall height of the image.
            else if (i > 0 && i < height - 1)
            {
                // Left edge of image
                if (j == 0)
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i + 1][j].rgbtRed +
                                                    temp_img[i - 1][j].rgbtRed +
                                                    temp_img[i + 1][j + 1].rgbtRed +
                                                    temp_img[i][j + 1].rgbtRed +
                                                    temp_img[i - 1][j + 1].rgbtRed) / 6.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i + 1][j].rgbtGreen +
                                                      temp_img[i - 1][j].rgbtGreen +
                                                      temp_img[i + 1][j + 1].rgbtGreen +
                                                      temp_img[i][j + 1].rgbtGreen +
                                                      temp_img[i - 1][j + 1].rgbtGreen) / 6.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i + 1][j].rgbtBlue +
                                                     temp_img[i - 1][j].rgbtBlue +
                                                     temp_img[i + 1][j + 1].rgbtBlue +
                                                     temp_img[i][j + 1].rgbtBlue +
                                                     temp_img[i - 1][j + 1].rgbtBlue) / 6.0);
                }
                // Right edge of image
                else if (j == width - 1)
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i + 1][j].rgbtRed +
                                                    temp_img[i - 1][j].rgbtRed +
                                                    temp_img[i + 1][j - 1].rgbtRed +
                                                    temp_img[i][j - 1].rgbtRed +
                                                    temp_img[i - 1][j - 1].rgbtRed) / 6.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i + 1][j].rgbtGreen +
                                                      temp_img[i- 1][j].rgbtGreen +
                                                      temp_img[i + 1][j - 1].rgbtGreen +
                                                      temp_img[i][j - 1].rgbtGreen +
                                                      temp_img[i - 1][j - 1].rgbtGreen) / 6.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i + 1][j].rgbtBlue +
                                                     temp_img[i - 1][j].rgbtBlue +
                                                     temp_img[i + 1][j - 1].rgbtBlue +
                                                     temp_img[i][j - 1].rgbtBlue +
                                                     temp_img[i - 1][j - 1].rgbtBlue) / 6.0);
                }
                // Pixels which are not confined by edge cases
                else
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i][j - 1].rgbtRed +
                                                    temp_img[i][j + 1].rgbtRed +
                                                    temp_img[i + 1][j - 1].rgbtRed +
                                                    temp_img[i + 1][j + 1].rgbtRed +
                                                    temp_img[i + 1][j].rgbtRed +
                                                    temp_img[i - 1][j - 1].rgbtRed +
                                                    temp_img[i - 1][j + 1].rgbtRed +
                                                    temp_img[i - 1][j].rgbtRed) / 9.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i][j - 1].rgbtGreen +
                                                      temp_img[i][j + 1].rgbtGreen +
                                                      temp_img[i + 1][j - 1].rgbtGreen +
                                                      temp_img[i + 1][j + 1].rgbtGreen +
                                                      temp_img[i + 1][j].rgbtGreen +
                                                      temp_img[i - 1][j - 1].rgbtGreen +
                                                      temp_img[i - 1][j + 1].rgbtGreen +
                                                      temp_img[i - 1][j].rgbtGreen) / 9.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i][j - 1].rgbtBlue +
                                                     temp_img[i][j + 1].rgbtBlue +
                                                     temp_img[i + 1][j - 1].rgbtBlue +
                                                     temp_img[i + 1][j + 1].rgbtBlue +
                                                     temp_img[i + 1][j].rgbtBlue +
                                                     temp_img[i - 1][j - 1].rgbtBlue +
                                                     temp_img[i - 1][j + 1].rgbtBlue +
                                                     temp_img[i - 1][j].rgbtBlue) / 9.0);
                }
            }
            // Same as when handeling the edge cases at the top of the image, now you have to do the same for the bottom corners and bottom edge.
            else if (i == height - 1)
            {
                // Left bottom Corner
                if (j == 0)
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i - 1][j].rgbtRed +
                                                    temp_img[i][j + 1].rgbtRed +
                                                    temp_img[i - 1][j + 1].rgbtRed) / 4.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i - 1][j].rgbtGreen +
                                                      temp_img[i][j + 1].rgbtGreen +
                                                      temp_img[i - 1][j + 1].rgbtGreen) / 4.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i - 1][j].rgbtBlue +
                                                     temp_img[i][j + 1].rgbtBlue +
                                                     temp_img[i - 1][j + 1].rgbtBlue) / 4.0);
                }
                // Right bottom corner
                else if (j == width - 1)
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i - 1][j].rgbtRed +
                                                    temp_img[i][j - 1].rgbtRed +
                                                    temp_img[i - 1][j - 1].rgbtRed) / 4.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i - 1][j].rgbtGreen +
                                                      temp_img[i][j - 1].rgbtGreen +
                                                      temp_img[i - 1][j - 1].rgbtGreen) / 4.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i - 1][j].rgbtBlue +
                                                     temp_img[i][j - 1].rgbtBlue +
                                                     temp_img[i - 1][j - 1].rgbtBlue) / 4.0);
                }
                // Bottom edge
                else
                {
                    image[i][j].rgbtRed = round((
                                                    temp_img[i][j].rgbtRed +
                                                    temp_img[i - 1][j].rgbtRed +
                                                    temp_img[i][j - 1].rgbtRed +
                                                    temp_img[i - 1][j - 1].rgbtRed +
                                                    temp_img[i][j + 1].rgbtRed +
                                                    temp_img[i - 1][j + 1].rgbtRed) / 6.0);
                    image[i][j].rgbtGreen = round((
                                                      temp_img[i][j].rgbtGreen +
                                                      temp_img[i - 1][j].rgbtGreen +
                                                      temp_img[i][j - 1].rgbtGreen +
                                                      temp_img[i - 1][j - 1].rgbtGreen +
                                                      temp_img[i][j + 1].rgbtGreen +
                                                      temp_img[i - 1][j + 1].rgbtGreen) / 6.0);
                    image[i][j].rgbtBlue = round((
                                                     temp_img[i][j].rgbtBlue +
                                                     temp_img[i - 1][j].rgbtBlue +
                                                     temp_img[i][j - 1].rgbtBlue +
                                                     temp_img[i - 1][j - 1].rgbtBlue +
                                                     temp_img[i][j + 1].rgbtBlue +
                                                     temp_img[i - 1][j + 1].rgbtBlue) / 6.0);
                }
            }
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    // Define sobel matrix
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // define all my x and y sobel variables and set them equal to 0
            float gxblue;
            float gyblue;
            float gxgreen;
            float gygreen;
            float gxred;
            float gyred;

            gxblue = gxgreen = gxred = gyblue = gygreen = gyred = 0;

            // Check if the pixel is outside the row and column. Otherwise add to the sobel sum
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if (i + r < 0 || i + r > height - 1)
                    {
                        continue;
                    }
                    if (j + c < 0 || j + c > width - 1)
                    {
                        continue;
                    }

                    gxblue += image[i + r][j + c].rgbtBlue * gx[r + 1][c + 1];
                    gyblue += image[i + r][j + c].rgbtBlue * gy[r + 1][c + 1];
                    gxgreen += image[i + r][j + c].rgbtGreen * gx[r + 1][c + 1];
                    gygreen += image[i + r][j + c].rgbtGreen * gy[r + 1][c + 1];
                    gxred += image[i + r][j + c].rgbtRed * gx[r + 1][c + 1];
                    gyred += image[i + r][j + c].rgbtRed * gy[r + 1][c + 1];
                }
            }

            // Implement sobel algorythm
            int blue = round(sqrt(gxblue * gxblue + gyblue * gyblue));
            int green = round(sqrt(gxgreen * gxgreen + gygreen * gygreen));
            int red = round(sqrt(gxred * gxred + gyred * gyred));

            // Check if the rgb values of pixels are below 255, if not, default to 255
            temp[i][j].rgbtBlue = blue > 255 ? 255 : blue;
            temp[i][j].rgbtGreen = green > 255 ? 255 : green;
            temp[i][j].rgbtRed = red > 255 ? 255 : red;
        }
    }

    // Finally, apply the filter to the original image.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }

    return;
}
