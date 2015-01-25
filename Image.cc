/**
 *  This file contains the definitions of the `Image` class.
 *
 *  The `Image` class was declared in `Image.h`.
 */
#include "Image.h"
#include <cassert>
#include "util.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;
using namespace util;

typedef uint8_t byte;

// Default constructor
Image::Image()
{
}

// Copy constructor
Image::Image(const Image& image)
{
    mMat = image.mMat.clone();
}

// Destructor
Image::~Image()
{
}


// Open an image file
Image Image::open(const string& filename)
{
    Image i;
    i.mMat = imread(filename, CV_LOAD_IMAGE_COLOR);
    return i;
}

// Save an image to disk
void Image::save(const std::string& filename) const
{
    imwrite(filename, mMat);
}


// Getters
long Image::cols() const { return  mMat.cols; }
long Image::rows() const { return  mMat.rows; }
long Image::step() const { return  mMat.step; }
short Image::channels() const { return  mMat.channels(); }
short Image::bps() const { return  sizeof(*mMat.data) * 8; }
byte* Image::data() const { return  mMat.data; }
long Image::max() const { return (cols() * (rows() - 1)) / 8; }


// Display an image using OpenCV
void  Image::show(int msecs) const
{
    namedWindow("Image", CV_WINDOW_NORMAL);
    imshow("Image", mMat);
    waitKey(msecs);
}


// Steg
Image& Image::steg(const string& text, const string& key)
{
    assert(cols() >= 110);
    assert(text.size() < max());
    assert(key.size() > 0);

    set_key(key);
    conceal(text);

    return *this;
}

// Unsteg
string Image::unsteg(const string& key) const
{
    assert(util::sha(key).size() == 40);

    // Verify password
    assert(hash().size() == 40);
    assert(util::sha(key) == hash());

    return reveal();
}

// Set a key for the image
void Image::set_key(const string& key)
{
    assert(key.size() > 0);

    string hash = util::sha(key);

    auto p = mMat.ptr<uchar>(0); // p points to the 0th row
    auto iter = hash.begin();

    int i = 0;
    while (i < (40 * 8)) {
        uchar c = *iter++;
        for (int j = 0; j < 8; ++j, ++i)
            setbit(p[i], getbit(c, j));
    }
}


// Conceal
void Image::conceal(const string& text)
{
    auto mit = mMat.begin<uchar>() + cols();

    for (auto sit = text.begin(); sit != text.end(); ++sit) {
        for (int i = 0; i < 8; ++i, ++mit) {
            setbit(*mit, getbit(*sit, i)); 

            if (*mit == 0)
                *mit = 2;
        }
    }

    *mit = 0;
}

// Reveal the hidden text
string Image::reveal() const
{
    string text;

    uchar c;
    auto mit = mMat.begin<uchar>() + cols();
    while (mit != mMat.end<uchar>()) {
        if (*mit == 0)
            break;

        for (int i = 0; i < 8; ++i, ++mit)
            setbit(c, getbit(*mit), i);

        text.push_back(c);
    }

    return text;
}

// Get the hash string of the key
string Image::hash() const
{
    string hashstr;

    auto p = mMat.ptr<uchar>(0);

    int i = 0;
    while (i < (40 * 8)) {
        uchar c;
        for (int j = 0; j < 8; ++j)
            setbit(c, getbit(p[i++]), j);

        hashstr.push_back(c);
    }

    return hashstr;
}
