#include <iostream>
#include <vector>

class ImageProcessor
{
private:
    std::string type;
    int width, height, rgb;
    std::vector<std::vector<std::vector<int>>> paddedInputImage;

public:
    ImageProcessor(const std::string &relativeImagePathFromMain);

    void getImage(const std::string &relativeImagePathFromMain);
    void generateImage(const std::vector<std::vector<std::vector<int>>> &paddedOutputImageMatrix, const std::string &relativeOutputPathFromMain);

    void sobelFilter(const std::string &relativeOutputPathFromMain);
    int multipleEachCellWithFilterCell(int row, int col, const std::vector<std::vector<int>> &grayedImage, const std::vector<std::vector<int>> &filter);

    void blueFilter(const std::string &relativeOutputPathFromMain);
};
