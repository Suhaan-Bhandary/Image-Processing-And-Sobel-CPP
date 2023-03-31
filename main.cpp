#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <math.h>
using namespace std;

void createBlackWhiteGradientImage()
{
    ofstream image;
    image.open("images/output/black_white_gradient.ppm");

    if (!image.is_open())
    {
        cout << "Error Opening the Image" << endl;
        return;
    }

    // Image Header
    image << "P3" << endl;
    image << "250 250" << endl;
    image << "255" << endl;

    // Image Body with 250 * 250 pixels
    for (int row = 0; row < 250; row++)
    {
        for (int col = 0; col < 250; col++)
        {
            image << col << " " << col << " " << col << endl;
        }
    }

    image.close();
}

void createRedGradientImage()
{
    ofstream image;
    image.open("images/output/red_gradient.ppm");

    if (!image.is_open())
    {
        cout << "Error Opening the Image" << endl;
        return;
    }

    // Image Header
    image << "P3" << endl;
    image << "250 250" << endl;
    image << "255" << endl;

    // Image Body with 250 * 250 pixels
    for (int row = 0; row < 250; row++)
    {
        for (int col = 0; col < 250; col++)
        {
            image << col << " " << 0 << " " << 0 << endl;
        }
    }

    image.close();
}

void createRandomImage()
{
    ofstream image;
    image.open("images/output/random.ppm");

    if (!image.is_open())
    {
        cout << "Error Opening the Image" << endl;
        return;
    }

    // Image Header
    image << "P3" << endl;
    image << "250 250" << endl;
    image << "255" << endl;

    // Seeding random
    srand(time(NULL));

    // Image Body with 250 * 250 pixels
    for (int row = 0; row < 250; row++)
    {
        for (int col = 0; col < 250; col++)
        {
            image << rand() % 256 << " " << rand() % 256 << " " << rand() % 256 << endl;
        }
    }

    image.close();
}

void blueFilter()
{
    ifstream image;
    ofstream newImage;
    image.open("images/input/flower.ppm");
    newImage.open("images/output/blueFilter.ppm");

    if (!image.is_open() || !newImage.is_open())
    {
        cout << "Error Opening Images" << endl;
        return;
    }

    // Copy Image Header and insert it in new image
    string type = "", width = "", height = "", rgb = "";
    image >> type >> width >> height >> rgb;

    // Create a image matrix to store all pixels

    newImage << type << endl;
    newImage << width << " " << height << endl;
    newImage << rgb << endl;

    // Image Body
    while (!image.eof())
    {
        int r, g, b;
        image >> r >> g >> b;
        newImage << r << " " << g << " " << (b + 100 >= 256 ? 255 : b + 100) << endl;
    }

    image.close();
    newImage.close();
}

int multipleEachCellWithFilterCell(int row, int col, vector<vector<int>> &imageMatrix, vector<vector<int>> &filter)
{
    int topRow = row - 1, topCol = col - 1;
    int n = 3;

    int value = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            value += imageMatrix[topRow + i][topCol + j] * filter[i][j];
        }
    }

    return value;
}

void sobelFilter()
{
    ifstream image;
    ofstream newImage;
    image.open("images/input/flower.ppm");
    newImage.open("images/output/sobelFilter.ppm");

    if (!image.is_open() || !newImage.is_open())
    {
        cout << "Error Opening Images" << endl;
        return;
    }

    // Copy Image Header and insert it in new image
    string type = "";
    int width, height, rgb;
    image >> type >> width >> height >> rgb;

    newImage << type << endl;
    newImage << width << " " << height << endl;
    newImage << rgb << endl;

    // Create a matrix representation of a image, rows denotes the number of pixar in height and cols for width
    int rows = height, cols = width;

    // Create a matrix with padding to store the values
    vector<vector<int>> grayMatrix(rows + 2, vector<int>(cols + 2, 0));

    // Leave the padding adn take input
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            int r, g, b;
            image >> r >> g >> b;

            grayMatrix[i][j] = (r + g + b) / 3;
        }
    }

    // Sobel Matrix
    vector<vector<int>> horizontalSobel = {
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1},
    };

    vector<vector<int>> verticalSobel = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1},
    };

    // Multiple the matrix with current box and replace the values
    for (int i = 1; i <= rows; i++)
    {
        for (int j = 1; j <= cols; j++)
        {
            // Get the multiplication of the matrix for rgb
            int horizontal = multipleEachCellWithFilterCell(i, j, grayMatrix, horizontalSobel);
            int vertical = multipleEachCellWithFilterCell(i, j, grayMatrix, verticalSobel);
            int g = sqrt(horizontal * horizontal + vertical * vertical);

            // Copying the filtered pixel result to the image
            newImage << g << " " << g << " " << g << endl;
        }
    }

    image.close();
    newImage.close();
}

int main()
{
    // Image Creation
    createBlackWhiteGradientImage();
    createRedGradientImage();
    createRandomImage();

    // Image Filter
    blueFilter();
    sobelFilter();

    return 0;
}