/** @file
 *  This file contains the definitions of the `MatImage` class.
 *
 *  The `MatImage` class was declared in `MatImage.h`.
 */
#include "MatImage.h"
#include <cassert>
#include "util.h"
#include <string>

using namespace cv;
using namespace std;
using namespace util;
using Glib::RefPtr;
using Gdk::Pixbuf;

typedef uint8_t byte;

// Default constructor
MatImage::MatImage()
{
}

// Open an image file
MatImage::MatImage(const string& filename)
{
    mMat = imread(filename, CV_LOAD_IMAGE_COLOR);
    cvtColor(mMat, mMat, CV_BGR2RGB);
}

// Create from Pixbuf
MatImage::MatImage(const RefPtr<Pixbuf>& p)
{
    RefPtr<Pixbuf> pp = p->copy();

    mMat = Mat(
            Size(pp->get_width(), pp->get_height()), // Dimensions
            CV_8UC3,                                 // Type
            pp->get_pixels(),                        // Pointer to data
            pp->get_rowstride());                    // Bytes per row
}

// Create from Mat
MatImage::MatImage(const Mat& mat)
{
    mMat = mat.clone();
}

// Copy constructor
MatImage::MatImage(const MatImage& image)
{
    mMat = image.mMat.clone();
}

// Destructor
MatImage::~MatImage()
{
}

//==========

// Save an image to disk
void MatImage::save(const string& filename) const
{
    // Convert the Mat from RGB to BGR before saving
    Mat bgr;
    cvtColor(mMat, bgr, CV_RGB2BGR);

    imwrite(filename, bgr);
}


// Getters
long MatImage::cols() const { return  mMat.cols; }
long MatImage::rows() const { return  mMat.rows; }
long MatImage::step() const { return  mMat.step; }
short MatImage::channels() const { return  mMat.channels(); }
short MatImage::bps() const { return  sizeof(*mMat.data) * 8; }
byte* MatImage::data() const { return  mMat.data; }
long MatImage::max() const { return (cols() * (rows() - 1)) / 8; }
bool MatImage::empty() const { return mMat.empty(); }


// Convert to a Pixbuf
RefPtr<Pixbuf> MatImage::pixbuf() const
{
    // Create a Pixbuf object and return it
    return Gdk::Pixbuf::create_from_data(
            mMat.data,              // Pointer to the pixels array
            Gdk::COLORSPACE_RGB,    // Colorspace (only RGB supported)
            false,                  // Has transparency?
            8,                      // Bits per sample (only 8 supported)
            mMat.cols,              // Width
            mMat.rows,              // Height
            mMat.step);             // Bytes per row (aka. rowstride or step)
}

// Scale the image to given width and height
RefPtr<Pixbuf> MatImage::scale(int width, int height) const
{
    assert(width > 0 or height > 0);

    double ratio = ((double) cols()) / rows();

    if (width <= 0)
        width = height * ratio;
    else if (height <= 0)
        height = width / ratio;

    return this->pixbuf()->scale_simple(width, height, Gdk::INTERP_BILINEAR);
}

// Returns a scaled image that fits within given dimenstion
RefPtr<Pixbuf> MatImage::fit(int width, int height) const
{
    assert(width > 0 and height > 0);

    double win_ratio = ((double) width) / height;
    double img_ratio = ((double) cols()) / rows();

    if (win_ratio > img_ratio)
        return this -> scale(0, height);
    else
        return this -> scale(width, 0);
}


// Display an image using OpenCV
void  MatImage::show(int msecs) const
{
    namedWindow("MatImage", CV_WINDOW_NORMAL);
    imshow("MatImage", mMat);
    waitKey(msecs);
}


// Steg
MatImage& MatImage::steg(const string& text, const string& key)
{
    assert(cols() >= 110);
    assert(text.size() < max());
    assert(key.size() > 0);

    set_key(key);
    conceal(text);

    return *this;
}

// Unsteg
string MatImage::unsteg(const string& key) const
{
    assert(util::sha(key).size() == 40);

    // Verify password
    assert(hash().size() == 40);
    assert(util::sha(key) == hash());

    return reveal();
}

// Set a key for the image
void MatImage::set_key(const string& key)
{
    assert(key.size() > 0);

    string hash = util::sha(key);

    uchar* p = mMat.ptr<uchar>(0); // p points to the 0th row
    string::iterator iter = hash.begin();

    int i = 0;
    while (i < (40 * 8)) {
        uchar c = *iter++;
        for (int j = 0; j < 8; ++j, ++i)
            setbit(p[i], getbit(c, j));
    }
}


// PRIVATE HELPERS:

// Conceal
void MatImage::conceal(const string& text)
{
    MatIterator_<uchar> mit = mMat.begin<uchar>() + cols();

    string::const_iterator sit;
    for (sit = text.begin(); sit != text.end(); ++sit) {
        for (int i = 0; i < 8; ++i, ++mit) {
            setbit(*mit, getbit(*sit, i)); 

            if (*mit == 0)
                *mit = 2;
        }
    }

    *mit = 0;
}

// Reveal the hidden text
string MatImage::reveal() const
{
    string text;

    uchar c;
    MatConstIterator_<uchar> mit = mMat.begin<uchar>() + cols();
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
string MatImage::hash() const
{
    string hashstr;

    const uchar* p = mMat.ptr<uchar>(0);

    int i = 0;
    while (i < (40 * 8)) {
        uchar c;
        for (int j = 0; j < 8; ++j)
            setbit(c, getbit(p[i++]), j);

        hashstr.push_back(c);
    }

    return hashstr;
}
