#include "src/ImageProcessor/ImageProcessor.h"

int main()
{
    ImageProcessor processor("./src/assets/images/input/flower.ppm");
    processor.sobelFilter("./src/assets/images/output/flower-sobel.ppm");
    processor.blueFilter("./src/assets/images/output/flower-blue.ppm");

    return 0;
}