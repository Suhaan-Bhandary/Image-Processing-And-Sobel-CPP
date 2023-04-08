#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(const std::string &relativeImagePathFromMain)
{
    getImage(relativeImagePathFromMain);
}

void ImageProcessor::getImage(const std::string &relativeImagePathFromMain)
{
    std::cout << "Reading Image from: " << relativeImagePathFromMain << std::endl;

    std::ifstream image(relativeImagePathFromMain);

    if (!image.is_open())
    {
        std::cout << "Error Reading the Image" << std::endl;
        exit(1);
    }

    // Reading Image
    image >> type >> width >> height >> rgb;

    // Create the image matrix
    int rows = height, cols = width;

    // Create a matrix with padding to store the input matrix
    paddedInputImage = std::vector<std::vector<std::vector<int>>>(rows + 2, std::vector<std::vector<int>>(cols + 2, {0, 0, 0}));

    // Leaving the padding and taking input
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            int r, g, b;
            image >> r >> g >> b;

            paddedInputImage[i][j] = {r, g, b};
        }
    }

    // Closing the image after reading
    image.close();
}

void ImageProcessor::generateImage(const std::vector<std::vector<std::vector<int>>> &paddedOutputImageMatrix, const std::string &relativeOutputPathFromMain)
{
    std::cout << "Generating Image at: " << relativeOutputPathFromMain << std::endl;

    std::ofstream outputImage(relativeOutputPathFromMain);
    if (!outputImage.is_open())
    {
        std::cout << "Error Writing the Image" << std::endl;
        exit(1);
    }

    // Writing header to the output image
    outputImage << type << std::endl;
    outputImage << width << " " << height << std::endl;
    outputImage << rgb << std::endl;

    // Writing pixels to the output image
    // Leave the padding adn take input
    int rows = height, cols = width;

    // We leave the first and last of row and col as both are padded
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            // Copying the pixels from matrix to outputImage
            outputImage << paddedOutputImageMatrix[i][j][0] << " "
                        << paddedOutputImageMatrix[i][j][1] << " "
                        << paddedOutputImageMatrix[i][j][2] << std::endl;
        }
    }

    outputImage.close();
}

int ImageProcessor::multipleEachCellWithFilterCell(int row, int col, const std::vector<std::vector<int>> &grayedImage, const std::vector<std::vector<int>> &filter)
{
    int topRow = row - 1, topCol = col - 1;
    int n = 3;

    int value = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            value += grayedImage[topRow + i][topCol + j] * filter[i][j];
        }
    }

    return value;
}

void ImageProcessor::sobelFilter(const std::string &relativeOutputPathFromMain)
{
    std::cout << "Applying Sobel Filter" << std::endl;

    // Creating a matrix to store the output
    // Same dimensions as the input matrix
    int rows = height, cols = width;
    std::vector<std::vector<int>> paddedGrayedImage(rows + 2, std::vector<int>(cols + 2, 0));

    // Convert the input matrix to gray scale in paddedGrayedImage
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            std::vector<int> pixel = paddedInputImage[i][j];
            int grayedPixelValue = (pixel[0] + pixel[1] + pixel[2]) / 3;

            paddedGrayedImage[i][j] = grayedPixelValue;
        }
    }

    // Applying sobel on the grayed image
    // Sobel Matrix
    std::vector<std::vector<int>> horizontalSobel = {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1},
    };

    std::vector<std::vector<int>> verticalSobel = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };

    // Padded Output Matrix to store the output pixels
    std::vector<std::vector<std::vector<int>>> paddedOutputImage(rows + 2, std::vector<std::vector<int>>(cols + 2, {0, 0, 0}));

    // Multiple the matrix with current box and replace the values
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            // Get the multiplication of the matrix for rgb
            int horizontal = multipleEachCellWithFilterCell(i, j, paddedGrayedImage, horizontalSobel);
            int vertical = multipleEachCellWithFilterCell(i, j, paddedGrayedImage, verticalSobel);

            int g = sqrt(horizontal * horizontal + vertical * vertical);

            // Converting the pixel value to Black or white
            int newG = g >= 100 ? 255 : 0;

            // Copying the filtered pixel result to the image
            paddedOutputImage[i][j] = {newG, newG, newG};
        }
    }

    // Finally output the matrix to a file
    generateImage(paddedOutputImage, relativeOutputPathFromMain);
}

void ImageProcessor::blueFilter(const std::string &relativeOutputPathFromMain)
{
    std::cout << "Applying Blue Filter" << std::endl;

    int rows = height, cols = width;

    // Padded Output Matrix to store the output pixels
    std::vector<std::vector<std::vector<int>>> paddedOutputImage(rows + 2, std::vector<std::vector<int>>(cols + 2, {0, 0, 0}));

    // Applying blue filter
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            int r = paddedInputImage[i][j][0];
            int g = paddedInputImage[i][j][1];
            int b = paddedInputImage[i][j][2] + 100 >= 256 ? 255 : paddedInputImage[i][j][2] + 100;

            // Copying the filtered pixel result to the image
            paddedOutputImage[i][j] = {r, g, b};
        }
    }

    // Finally output the matrix to a file
    generateImage(paddedOutputImage, relativeOutputPathFromMain);
}