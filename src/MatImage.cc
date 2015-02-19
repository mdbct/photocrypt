/** @file
 *
 *  This file contains the definitions of the `MatImage` class.
 *
 *  The `MatImage` class was declared in `MatImage.h`.
 */
#include "MatImage.h"
#include <cassert>
#include "util.h"
#include <string>
#include "Error.h"
#include <iostream>

using namespace cv;
using namespace std;
using Glib::RefPtr;
using Gdk::Pixbuf;

using byte = uint8_t;
using uint = unsigned int;

namespace Photocrypt
{

// Default constructor
MatImage::MatImage()
{
}

// Open an image file
MatImage::MatImage(const string& filename)
{
    mMat = imread(filename, CV_LOAD_IMAGE_COLOR);

    if (not mMat.data)
        throw IOError("Couldn't open image file");

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
long MatImage::cols() const
{
    return mMat.cols;
}

long MatImage::rows() const
{
    return mMat.rows;
}

long MatImage::step() const
{
    return mMat.step;
}

short MatImage::channels() const
{
    return mMat.channels();
}

short MatImage::bps() const
{
    return  sizeof(*mMat.data) * 8;
}

byte* MatImage::data() const
{
    return  mMat.data;
}

long MatImage::max() const
{
    return (cols() * (rows() - 1)) / 3;
}

bool MatImage::empty() const
{
    return mMat.empty();
}

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
            mMat.step               // Bytes per row (aka. rowstride or step)
            );
}

// Scale the image to given width and height
RefPtr<Pixbuf> MatImage::scale(int width, int height) const
{
    if (width <= 0 and height <= 0)
        throw invalid_argument("Both arguments can't be zero simultaneously.");

    double ratio = static_cast<double>(cols()) / rows();

    if (width <= 0)
        width = height * ratio;
    else if (height <= 0)
        height = width / ratio;

    return this->pixbuf()->scale_simple(width, height, Gdk::INTERP_BILINEAR);
}

// Returns a scaled image that fits within given dimenstion
RefPtr<Pixbuf> MatImage::fit(int width, int height) const
{
    if (width <= 0 or height <= 0)
        throw invalid_argument("Both arguments should be greater than zero");

    double win_ratio = static_cast<double>(width) / height;
    double img_ratio = static_cast<double>(cols()) / rows();

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
    if (empty())
        throw ImageEmptyError();

    if (text.empty())
        throw TextEmptyError();

    if (key.empty())
        throw KeyEmptyError();

    if (cols() < 80)
        throw InsufficientImageError("Image is too small");

    if (text.size() >= max())
        throw InsufficientImageError("Text is too large for this image");

    set_key(key);
    conceal(text, key);

    return (*this);
}

// Unsteg
string MatImage::unsteg(const string& key) const
{
    assert(sha(key).size() == 20);

    if (empty())
        throw ImageEmptyError();

    if (cols() < 80)
        throw InsufficientImageError("Image is not stego");

    if (sha(key) != hash(key))
        throw KeyMismatchError();

    return reveal(key);
}


/* PRIVATE HELPERS */

// Set a key for the image
void MatImage::set_key(const string& key)
{
    if (key.empty())
        throw KeyEmptyError("Key is empty");

    string hash = sha(key);
    assert(hash.size() == 20);
    hash.push_back(0);

    auto kit = key.begin();

    auto mit = mMat.begin<Vec3b>();
    for (auto hit = hash.begin(); hit != hash.end(); ++hit)
    {
        // Get ignore bit
        int ignore = static_cast<uint>(*(kit++)) % 8;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c = static_cast<uchar>(*hit);
        int i = 0, b = 0;

        for (int px = 0; px < 3; ++px, ++mit) {
            for (int color = 0; color < 3; ++color, ++i) {
                if (i != ignore) {
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    setbit( (*mit)[color], getbit(c, b++), s);
                }
            }
        }
    }
}

// Conceal
void MatImage::conceal(const string& text, const string& key)
{
    if (text.empty())
        throw TextEmptyError();

    if (key.empty())
        throw KeyEmptyError();

    auto mit = mMat.begin<Vec3b>() + cols();
    auto txt = text;
    txt.push_back(0);

    auto kit = key.begin();

    for (auto tit = txt.begin(); tit != txt.end(); ++tit) {

        // Get ignore bits
        int ignore = static_cast<uint>(*(kit++)) % 8;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c = static_cast<uchar>(*tit);
        int i = 0, b = 0;

        for (int px = 0; px < 3; ++px, ++mit) {
            for (int color = 0; color < 3; ++color, ++i) {
                if (i != ignore) {
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    setbit( (*mit)[color], getbit(c, b++), s);
                }
            }
        }
    }
}

// Reveal the hidden text
string MatImage::reveal(const string& key) const
{
    string text;
    auto kit = key.begin();

    for (auto mit = mMat.begin<Vec3b>() + cols(); ;) {

        // Get ignore bits
        int ignore = static_cast<uint>(*(kit++)) % 8;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c;
        int b = 0, i = 0;

        for (int px = 0; px < 3; ++px, ++mit) {
            for (int color = 0; color < 3; ++color, ++i) {
                if (i != ignore) {
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    setbit( c, getbit((*mit)[color], s), b++);
                }
            }
        }

        if (c == 0) break;
        text.push_back(c);
    }

    return text;
}

// Get the hash string of the key
string MatImage::hash(const string& key) const
{
    string hash;
    auto kit = key.begin();

    for (auto mit = mMat.begin<Vec3b>(); ;) {

        // Get ignore bits
        int ignore = static_cast<uint>(*(kit++)) % 8;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c;
        int b = 0, i = 0;

        // Iterate over 3 pixels for each byte
        for (int px = 0; px < 3; ++px, ++mit) {

            // Iterator over 3 colors for each pixel
            for (int color = 0; color < 3; ++color, ++i) {
                if (i != ignore) {
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    setbit( c, getbit((*mit)[color], s), b++ );
                }
            }
        }

        if (c == 0) break;
        hash.push_back(c);
    }

    return hash;
}

} // namespace Photocrypt
