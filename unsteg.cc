#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc != 2)
        throw length_error("Usage: ./unsteg <stego-image>");

    // Get the file names
    string stegoImageFileName = argv[1];

    // Load the image
    Mat stegoImage = imread(stegoImageFileName, CV_LOAD_IMAGE_GRAYSCALE);
    if (stegoImage.empty())
        throw runtime_error("Failed to load image.");

    // Get the size of string from the first three bytes
    MatIterator_<uchar> it = stegoImage.begin<uchar>();
    size_t size = 0;
    for (int i = 0; i < 10; ++i) {
        size += *it++;
    }

    // Get the string
    string text;
    for (int i = 0; i < size; ++i) {
        text.push_back(*it);
        it++;
    }

    cout << text << endl;
}
