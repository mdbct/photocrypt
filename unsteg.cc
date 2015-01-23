#include <iostream>
#include <cassert>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "encrypt.h"
#include "bit.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    assert(argc == 2);

    // Get the filenames:
    string imageFilename = argv[1];

    // Load the image:
    Mat I = imread(imageFilename, CV_LOAD_IMAGE_COLOR);
    assert(I.data);
    
    int n_rows = I.rows - 1;
    int n_cols = I.cols * I.channels();

    // Ask for password
    string password;
    cerr << "Enter the password: ";
    cin >> password;

    // Get SHA from image
    string sha;

    auto p = I.ptr<uchar>(0);

    int i = 0;
    while (i < (40 * 8))
    {
        uchar c;

        for (int j = 0; j < 8; ++j)
        {
            set_bit( c, get_bit(p[i], 0), j );
            ++i;
        }

        sha.push_back(c);
    }

    assert(sha.size() == 40);
    assert(encrypt(password).compare(sha) == 0);

    // Extract the text
    string text;

    MatIterator_<Vec3b> it = I.begin<Vec3b>() + n_cols;
    while (it != I.end<Vec3b>())
    {
        if ( (*it)[0] == 0 )
            break;

        uchar c;
        int   k = 0;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                // Ignore the 9th bit
                if (i == 2 and j == 2)
                    continue;

                set_bit( c,  get_bit( (*it)[j], 0), k++ );
            }

            it++;
        }

        text.push_back(c);
    }

    cout << text << endl;
}
