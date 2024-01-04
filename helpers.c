#include "helpers.h"
#include <math.h>

// Convert imageCopy to grayscale
void grayscale(int height, int width, RGBTRIPLE imageCopy[height][width])
{
    float average;
    int roundedAverage;
    // loop through each pixel of the imageCopy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calculate the average for each pixel and round it
            average = (float) (imageCopy[i][j].rgbtBlue + imageCopy[i][j].rgbtGreen + imageCopy[i][j].rgbtRed) / 3.0;
            roundedAverage = round(average);
            // set each color value to the average value
            imageCopy[i][j].rgbtBlue = roundedAverage;
            imageCopy[i][j].rgbtGreen = roundedAverage;
            imageCopy[i][j].rgbtRed = roundedAverage;
        }
    }
    return;
}

// Reflect imageCopy horizontally
void reflect(int height, int width, RGBTRIPLE imageCopy[height][width])
{
    // goal: for each row(width) swap pixels horizontally opposite sides

    // loop through the row of pixels
    // create a temp variable to store the first element
    // assign the first element to the one on the end (height length - 1)
    // assign the height length - 1 to temp

    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int leftElement = 0, rightElement = width - 1; leftElement < rightElement; leftElement++, rightElement--)
        {
            temp = imageCopy[i][leftElement];
            imageCopy[i][leftElement] = imageCopy[i][rightElement];
            imageCopy[i][rightElement] = temp;
        }
    }

    return;
}

// Blur imageCopy
void blur(int height, int width, RGBTRIPLE imageCopy[height][width])
{
    // Loop through the two dimensional
    // Locate a specific pixel from the height and width
    // Check if the number is valid position
    // Calculate the sum of the sorrounding numbers
    // Calculate the average
    // Handle boundary cases

    RGBTRIPLE temp[height][width];

    double sumRed;
    double sumBlue;
    double sumGreen;
    int count;

    float averageRed, averageBlue, averageGreen;

    int roundedAverageRed, roundedAverageGreen, roundedAverageBlue;

    // make a copy to preserve the original values from the imageCopy
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            temp[row][col] = imageCopy[row][col];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // reset the sum and count for every pixel before calculating the average
            // this is more efficient than declare them inside the loop, as this would create and destroy them every iteration
            sumRed = 0;
            sumBlue = 0;
            sumGreen = 0;
            count = 0;

            // loop through the sorrounding pixels
            for (int i = row - 1; i <= row + 1; i++)
            {
                for (int j = col - 1; j <= col + 1; j++)
                {
                    if (i < 0 || i > height || j < 0 || j > width)
                    {
                        continue;
                    }

                    if (i >= 0 && i < height && j >= 0 && j < width)
                    {
                        sumRed += temp[i][j].rgbtRed;
                        sumBlue += temp[i][j].rgbtBlue;
                        sumGreen += temp[i][j].rgbtGreen;
                        count++;
                    }
                }
            }

            averageRed = sumRed / count;
            averageBlue = sumBlue / count;
            averageGreen = sumGreen / count;

            roundedAverageRed = round(averageRed);
            roundedAverageGreen = round(averageGreen);
            roundedAverageBlue = round(averageBlue);

            imageCopy[row][col].rgbtRed = roundedAverageRed;
            imageCopy[row][col].rgbtGreen = roundedAverageGreen;
            imageCopy[row][col].rgbtBlue = roundedAverageBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Compute Gx and Gy for each channel of red, green and blue using the convulotional matrix or Sobel operator
    // For pixel at the border, treat any pixel past the border as having all 0 values
    // Compute each new channel value as the square root of Gx^2 + Gy^2

    int borderRows = height + 2;
    int borderCols = width + 2;

    RGBTRIPLE imageCopy[borderRows][borderCols];

    int gxRed;
    int gyRed;
    int newAmountRed;

    int gxGreen;
    int gyGreen;
    int newAmountGreen;

    int gxBlue;
    int gyBlue;
    int newAmountBlue;

    // handling edge pixels by treating the sorrounding as 0
    for (int row = 0; row < height + 2; row++)
    {
        for (int col = 0; col < width + 2; col++)
        {
            if (row == 0 || row == height + 1 || col == 0 || col == width + 1)
            {
                imageCopy[row][col].rgbtRed = 0;
                imageCopy[row][col].rgbtGreen = 0;
                imageCopy[row][col].rgbtBlue = 0;
            }
        }
    }

    // copy the original image to the temporary
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            imageCopy[row + 1][col + 1] = image[row][col];
        }
    }

    for (int row = 1; row < height + 1; row++)
    {
        for (int col = 1; col < width + 1; col++)
        {
            gxRed = 0;
            gyRed = 0;

            gxGreen = 0;
            gyGreen = 0;

            gxBlue = 0;
            gyBlue = 0;

            gxRed += -1 * imageCopy[row - 1][col - 1].rgbtRed + 0 * imageCopy[row - 1][col].rgbtRed +
                     1 * imageCopy[row - 1][col + 1].rgbtRed;
            gxRed += -2 * imageCopy[row][col - 1].rgbtRed + 0 * imageCopy[row][col].rgbtRed + 2 * imageCopy[row][col + 1].rgbtRed;
            gxRed += -1 * imageCopy[row + 1][col - 1].rgbtRed + 0 * imageCopy[row + 1][col].rgbtRed +
                     1 * imageCopy[row + 1][col + 1].rgbtRed;

            gyRed += -1 * imageCopy[row - 1][col - 1].rgbtRed - 2 * imageCopy[row - 1][col].rgbtRed -
                     1 * imageCopy[row - 1][col + 1].rgbtRed;
            gyRed += 0 * imageCopy[row][col - 1].rgbtRed + 0 * imageCopy[row][col].rgbtRed + 0 * imageCopy[row][col + 1].rgbtRed;
            gyRed += 1 * imageCopy[row + 1][col - 1].rgbtRed + 2 * imageCopy[row + 1][col].rgbtRed +
                     1 * imageCopy[row + 1][col + 1].rgbtRed;

            if (gxRed == 0 && gyRed == 0)
            {
                newAmountRed = 0;
            }
            else
            {
                newAmountRed = round(sqrt(pow(gxRed, 2) + pow(gyRed, 2)));
            }

            newAmountRed = (newAmountRed > 255) ? 255 : newAmountRed;

            image[row - 1][col - 1].rgbtRed = newAmountRed;

            gxGreen += -1 * imageCopy[row - 1][col - 1].rgbtGreen + 0 * imageCopy[row - 1][col].rgbtGreen +
                       1 * imageCopy[row - 1][col + 1].rgbtGreen;
            gxGreen +=
                -2 * imageCopy[row][col - 1].rgbtGreen + 0 * imageCopy[row][col].rgbtGreen + 2 * imageCopy[row][col + 1].rgbtGreen;
            gxGreen += -1 * imageCopy[row + 1][col - 1].rgbtGreen + 0 * imageCopy[row + 1][col].rgbtGreen +
                       1 * imageCopy[row + 1][col + 1].rgbtGreen;

            gyGreen += -1 * imageCopy[row - 1][col - 1].rgbtGreen - 2 * imageCopy[row - 1][col].rgbtGreen -
                       1 * imageCopy[row - 1][col + 1].rgbtGreen;
            gyGreen +=
                0 * imageCopy[row][col - 1].rgbtGreen + 0 * imageCopy[row][col].rgbtGreen + 0 * imageCopy[row][col + 1].rgbtGreen;
            gyGreen += 1 * imageCopy[row + 1][col - 1].rgbtGreen + 2 * imageCopy[row + 1][col].rgbtGreen +
                       1 * imageCopy[row + 1][col + 1].rgbtGreen;

            if (gxGreen == 0 && gyGreen == 0)
            {
                newAmountGreen = 0;
            }
            else
            {
                newAmountGreen = round(sqrt(pow(gxGreen, 2) + pow(gyGreen, 2)));
            }

            newAmountGreen = (newAmountGreen > 255) ? 255 : newAmountGreen;
            image[row - 1][col - 1].rgbtGreen = newAmountGreen;

            gxBlue += -1 * imageCopy[row - 1][col - 1].rgbtBlue + 0 * imageCopy[row - 1][col].rgbtBlue +
                      1 * imageCopy[row - 1][col + 1].rgbtBlue;
            gxBlue +=
                -2 * imageCopy[row][col - 1].rgbtBlue + 0 * imageCopy[row][col].rgbtBlue + 2 * imageCopy[row][col + 1].rgbtBlue;
            gxBlue += -1 * imageCopy[row + 1][col - 1].rgbtBlue + 0 * imageCopy[row + 1][col].rgbtBlue +
                      1 * imageCopy[row + 1][col + 1].rgbtBlue;

            gyBlue += -1 * imageCopy[row - 1][col - 1].rgbtBlue - 2 * imageCopy[row - 1][col].rgbtBlue -
                      1 * imageCopy[row - 1][col + 1].rgbtBlue;
            gyBlue +=
                0 * imageCopy[row][col - 1].rgbtBlue + 0 * imageCopy[row][col].rgbtBlue + 0 * imageCopy[row][col + 1].rgbtBlue;
            gyBlue += 1 * imageCopy[row + 1][col - 1].rgbtBlue + 2 * imageCopy[row + 1][col].rgbtBlue +
                      1 * imageCopy[row + 1][col + 1].rgbtBlue;

            if (gxBlue == 0 && gyBlue == 0)
            {
                newAmountBlue = 0;
            }
            else
            {
                newAmountBlue = round(sqrt(pow(gxBlue, 2) + pow(gyBlue, 2)));
            }

            newAmountBlue = (newAmountBlue > 255) ? 255 : newAmountBlue;
            image[row - 1][col - 1].rgbtBlue = newAmountBlue;
        }
    }


    return;
}
