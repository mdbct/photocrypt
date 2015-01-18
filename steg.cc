#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdexcept>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc != 3)
        throw invalid_argument("Usage: ./steg <image> <text-file>");

    // Get file names
    string inImageFileName = argv[1];
    string textFileName = argv[2];

    // Load the image file
    Mat inImage = imread(inImageFileName, CV_LOAD_IMAGE_GRAYSCALE);
    if (inImage.empty())
        throw runtime_error("Failed to load image.");

    // Load the text file
    ifstream textFile(textFileName);
    if (!textFile)
        throw runtime_error("Unable to open file: " + textFileName);

    // Put the contents of text file into a string
    string  text;
    char    c;
    while (textFile.get(c)) text.push_back(c);
    size_t  size = text.size();
    if (size >= (10 * 255))
        throw overflow_error("Too big text file");

    // Put the size of text in first 3 bytes
    Mat outImage = inImage.clone();
    MatIterator_<uchar> it = outImage.begin<uchar>();
    for (int i = 0; i < 10; ++i)
    {
        if (size > 255)
        {
            *it = 255;
            size -= 255;
        }
        else
        {
            *it = size % 256;
            size = 0;
        }
        it++;
    }

    // Put the text in rest of pixels
    copy(text.begin(), text.end(), it);

    // Save the output image
    string outImageFileName = "outImage.bmp";
    imwrite(outImageFileName, outImage);

    cout << "Stego image written to " + outImageFileName << endl;
}
