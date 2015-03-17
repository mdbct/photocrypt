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


// Open an image file
MatImage::MatImage(const string& filename)
{
    // Load the image from the given filename
    mMat = imread(filename, CV_LOAD_IMAGE_COLOR);

    // If something goes wrong
    if (not mMat.data)
        throw IOError("Couldn't open image file");

    // Convert the image from BGR to RGB.
    //
    // This is required because OpenCV loads image as BGR and gtk requires
    // the image in RGB to display correctly.
    cvtColor(mMat, mMat, CV_BGR2RGB);
}

// Create from Pixbuf
MatImage::MatImage(const RefPtr<Pixbuf>& p)
{
    // Copy the Pixbuf
    RefPtr<Pixbuf> pp = p->copy();

    // Convert into cv::Mat
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
    // We don't need to do anything here.
    //
    // That's because our only member data is a cv::Mat object.
    // And the destructor of cv::Mat will do the neccesary cleanups.
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


// Constant getters

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
    // Both parameter can't be zero
    if (width <= 0 and height <= 0)
        throw invalid_argument("Both arguments can't be zero simultaneously.");

    // Get the aspect ratio of the original image
    double ratio = static_cast<double>(cols()) / rows();

    // If either height or width is 0, then adjust it to maintain ratio
    if (width <= 0)
        width = height * ratio;
    else if (height <= 0)
        height = width / ratio;

    // Scale the image
    return this->pixbuf()->scale_simple(width, height, Gdk::INTERP_BILINEAR);
}

// Returns a scaled image that fits within given dimenstion
RefPtr<Pixbuf> MatImage::fit(int width, int height) const
{
    // the width or height cannot be less than 1
    if (width < 1 or height < 1)
        throw invalid_argument("Both arguments should be greater than zero");

    // Get the ratio of window
    double win_ratio = static_cast<double>(width) / height;
    // Get the ratio of image
    double img_ratio = static_cast<double>(cols()) / rows();

    // Fit it
    if (win_ratio > img_ratio)
        return this -> scale(0, height);
    else
        return this -> scale(width, 0);
}

// Display an image using OpenCV
void  MatImage::show(int msecs) const
{
    // Create a window
    namedWindow("MatImage", CV_WINDOW_NORMAL);

    // Display the image in that window
    imshow("MatImage", mMat);

    // Wait
    waitKey(msecs);
}


// Steg
MatImage& MatImage::steg(const string& text, const string& key)
{
    // Check for pre-conditions
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

    // If everything is valid, then steg it

    // First steg the key
    set_key(key);

    // Then steg the text
    conceal(text, key);

    return (*this);
}

// Unsteg
string MatImage::unsteg(const string& key) const
{
    // Check for pre-conditions
    assert(sha(key).size() == 20);

    if (empty())
        throw ImageEmptyError();

    if (cols() < 80)
        throw InsufficientImageError("Image is not stego");

    if (sha(key) != hash(key))
        throw KeyMismatchError();

    // Decrypt the text and return it
    return reveal(key);
}


/* PRIVATE HELPERS */

// Set a key for the image
void MatImage::set_key(const string& key)
{
    // Password should not be empty
    if (key.empty())
        throw KeyEmptyError("Key is empty");

    // We'll store the SHA1 digest of the key (not the key itself) in the image.
    string hash = sha(key);
    assert(hash.size() == 20);

    // Key will be used to choose the store bits and ignore color
    auto kit = key.begin();

    // The key (it's hash) will be written in the first row of image
    auto mit = mMat.begin<Vec3b>();

    // For every character (or byte) of hash
    for (auto hit = hash.begin(); hit != hash.end(); ++hit)
    {
        // Get the color to ignore
        int ignore = static_cast<uint>(*(kit++)) % 8;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c = static_cast<uchar>(*hit);
        int i = 0, b = 0;

        // 3 pixels for each byte
        for (int px = 0; px < 3; ++px, ++mit) {

            // 3 colors for each pixel
            for (int color = 0; color < 3; ++color, ++i) {

                // Don't use the ignore color
                if (i != ignore) {
                    // Get the store bit
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    // Store the bit
                    setbit( (*mit)[color], getbit(c, b++), s);
                }
            }
        }
    }
}

// Conceal
void MatImage::conceal(const string& text, const string& key)
{
    // Check for pre-conditions
    if (text.empty())
        throw TextEmptyError();

    if (key.empty())
        throw KeyEmptyError();

    // The text will be written starting from the 2nd row of the image
    auto mit = mMat.begin<Vec3b>() + cols();

    // Add '\0' to the text as an indicator for end of text while decrypting
    auto txt = text;
    txt.push_back(0);

    // Key will be used to choose store bits and ignore color
    auto kit = key.begin();

    // For each character (of byte) of text
    for (auto tit = txt.begin(); tit != txt.end(); ++tit) {

        // Get the ignore bit
        int ignore = static_cast<uint>(*(kit++)) % 9;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c = static_cast<uchar>(*tit);
        int i = 0, b = 0;

        // 3 pixels for each byte
        for (int px = 0; px < 3; ++px, ++mit) {

            // 3 colors for each pixel
            for (int color = 0; color < 3; ++color, ++i) {

                // Don't use the ignore color
                if (i != ignore) {
                    // Get the store bit
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    // Store the bit
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

    // Key will be used to choose store bits and ignore color
    auto kit = key.begin();

    // Start iterating the pixels from the 2nd row
    for (auto mit = mMat.begin<Vec3b>() + cols(); ;) {

        // Get the ignore color
        int ignore = static_cast<uint>(*(kit++)) % 9;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c;
        int b = 0, i = 0;

        // 3 pixels for each byte
        for (int px = 0; px < 3; ++px, ++mit) {

            // 3 colors for each pixel
            for (int color = 0; color < 3; ++color, ++i) {

                // Don't use the ignore color
                if (i != ignore) {
                    // Get the store bit
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    // Get the bit from the color
                    setbit( c, getbit((*mit)[color], s), b++);
                }
            }
        }

        // After getting each byte, check if it is the end of text
        if (c == 0) break;
        text.push_back(c);
    }

    return text;
}

// Get the hash string of the key
string MatImage::hash(const string& key) const
{
    string hash;

    // Key will be used to choose store bits and ignore color
    auto kit = key.begin();

    // Start iterating the pixels from 1st row
    auto mit = mMat.begin<Vec3b>();

    // The hash is 20 bytes long
    for (int size = 0; size < 20; ++size) {

        // Get the ignore color
        int ignore = static_cast<uint>(*(kit++)) % 8;
        if (kit == key.end()) kit = key.begin();

        // Set bits
        uchar c;
        int b = 0, i = 0;

        // 3 pixels for each byte
        for (int px = 0; px < 3; ++px, ++mit) {

            // 3 colors for each pixel
            for (int color = 0; color < 3; ++color, ++i) {

                // Don't use the ignore color
                if (i != ignore) {

                    // Get the store bit
                    int s = static_cast<uint>(*(kit++)) % 2;
                    if (kit == key.end()) kit = key.begin();

                    // Get the bit from the color
                    setbit( c, getbit((*mit)[color], s), b++ );
                }
            }
        }

        hash.push_back(c);
    }

    return hash;
}

} // namespace Photocrypt
