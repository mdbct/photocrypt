#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "encrypt.h"
#include "bit.h"

using namespace std;
using namespace cv;

string to_string(ifstream& in);

int main(int argc, char** argv)
{
    assert(argc == 3);

    // Get the file names
    string imagefile = argv[1];
    ifstream textfile(argv[2]);
    string outfile = "out.bmp";

    // Load the image
    Mat I = imread(imagefile, CV_LOAD_IMAGE_COLOR);
    assert(I.data);

    // Check the size of images
    assert(I.cols >= 110);

    long n_pixels = I.cols * (I.rows - 1); // available for hiding info
    long max_size = (3 * n_pixels ) / 8;

    // Check the size of textfile
    string text = to_string(textfile);
    cout << "max_size = " << max_size << endl;
    cout << "textsize = " << text.size() << endl;
    assert(text.size() <= max_size);

    // Ask to provide password
    string password, sha;
    cout << "Set a password: ";
    cin >> password;
    sha = encrypt(password);
    assert(sha.size() == 40);

    long n_rows = I.rows - 1;
    long n_cols = I.cols * I.channels();

    // Set the password in the first row
    auto p = I.ptr<uchar>(0);
    int  i = 0;
    auto iter = sha.begin();
    while (i < (40 * 8))
    {
        uchar c = *iter;
        for (int j = 0; j < 8; ++j)
        {
            assert(  set_bit(p[i], get_bit(c, j) )  );
            ++i;
        }
        ++iter;
    }

    // Fill
    MatIterator_<Vec3b> it = I.begin<Vec3b>() + n_cols;
    for (iter = text.begin(); iter != text.end(); iter++)
    {
        uchar c = *iter;
        int   k = 0;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (i == 2 and j == 2)
                    continue;

                set_bit( (*it)[j], get_bit(c, k++) );
            }

            if ( (*it)[0] == 0 ) (*it)[0] = 2;

            it++;
        }
    }

    (*it)[0] = 0;

    // Should be finished now
    imwrite(outfile, I);
    cout << "Stego image written to 'out.bmp'." << endl;
}

string to_string(ifstream& in)
{
    ostringstream out;

    char c;
    while (in.get(c))
        out.put(c);

    return out.str();
}
