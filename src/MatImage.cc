/** @file
 *  This file contains the definitions of the `MatImage` class.
 *
 *  The `MatImage` class was declared in `MatImage.h`.
 */
#include "MatImage.h"
#include <cassert>
#include "util.h"
#include <string>
#include "Error.h"

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
    return (cols() * (rows() - 1));
}

bool MatImage::empty() const
{
    return mMat.empty();
}

/*
bool MatImage::is_stego() const
{
    return (hash().size() == 40);
}
*/


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
    if (mMat.empty())
        throw ImageEmptyError();

    if (text.empty())
        throw TextEmptyError();

    if (key.empty())
        throw KeyEmptyError();

    if (cols() < 110)
        throw InsufficientImageError();

    if (text.size() >= max())
        throw InsufficientImageError();

    set_key(key);
    conceal(text, key);

    return (*this);
}

// Unsteg
string MatImage::unsteg(const string& key) const
{
    assert(sha(key).size() == 40);

    if (mMat.empty())
        throw ImageEmptyError();

    /*
    if (not is_stego())
       throw ImageNotStegoError();
    */

    if (sha(key) != hash(key))
        throw KeyMismatchError();

    return reveal(key);
}


/* PRIVATE HELPERS */

// Set a key for the image
void MatImage::set_key(const string& key)
{
    if (key.empty())
        throw KeyEmptyError();

    string hash = sha(key);
    assert(hash.size() == 40);
    hash.push_back(0);

    auto kit = key.begin();
    vector<int> ignore[3];

    auto mit = mMat.begin<Vec3b>();
    for (auto hit = hash.begin(); hit != hash.end(); ++hit, ++mit)
    {
        // Get ignore bits
        for (int i = 0; i < 3; ++i) {
            int kval = static_cast<int>(*kit);

            ignore[i] = { kval % 4 };
            if (i == 2) ignore[i].push_back( (kval+1) % 4 );

            kit++;
            if (kit == key.end()) kit = key.begin();
        }

        // Set bits
        uchar c = static_cast<uchar>(*hit);
        int   b = 0;
        
        for (int clr = 0; clr < 3; ++clr) {
            for (int i = 0; i < 4; ++i) {
                auto pos = find( ignore[clr].begin(), ignore[clr].end(), i );
                if (pos == ignore[clr].end()) {
                    setbit( (*mit)[clr], getbit(c, b++), i );
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

    vector<int> ignore[3];
    auto kit = key.begin();

    for (auto tit = txt.begin(); tit != txt.end(); ++tit, ++mit) {

        // Get ignore bits
        for (int i = 0; i < 3; ++i) {
            int kval = static_cast<int>(*kit);

            ignore[i] = { kval % 4 };
            if (i == 2) ignore[i].push_back( (kval+1) % 4 );

            kit++;
            if (kit == key.end()) kit = key.begin();
        }

        // Set bits
        uchar c = static_cast<uchar>(*tit);
        int   b = 0;
        
        for (int clr = 0; clr < 3; ++clr) {
            for (int i = 0; i < 4; ++i) {
                auto pos = find( ignore[clr].begin(), ignore[clr].end(), i );
                if (pos == ignore[clr].end()) {
                    setbit( (*mit)[clr], getbit(c, b++), i );
                }
            }
        }
    }
}

// Reveal the hidden text
string MatImage::reveal(const string& key) const
{
    /*
    if (not is_stego())
        throw ImageNotStegoError();
        */
    string text;
    vector<int> ignore[3];
    auto kit = key.begin();

    for (auto mit = mMat.begin<Vec3b>() + cols(); ; ++mit) {

        // Get ignore bits
        for (int i = 0; i < 3; ++i) {
            int kval = static_cast<int>(*kit);

            ignore[i] = { kval % 4 };
            if (i == 2) ignore[i].push_back( (kval+1) % 4 );

            kit++;
            if (kit == key.end()) kit = key.begin();
        }

        // Set bits
        uchar c;
        int b = 0;

        for (int clr = 0; clr < 3; ++clr) {
            for (int i = 0; i < 4; ++i) {
                auto pos = find( ignore[clr].begin(), ignore[clr].end(), i );
                if (pos == ignore[clr].end()) {
                    setbit( c, getbit( (*mit)[clr], i), b++ );
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
    vector<int> ignore[3];
    auto kit = key.begin();

    for (auto mit = mMat.begin<Vec3b>(); /*hash.size() != 40*/; ++mit) {

        // Get ignore bits
        for (int i = 0; i < 3; ++i) {
            int kval = static_cast<int>(*kit);

            ignore[i] = { kval % 4 };
            if (i == 2) ignore[i].push_back( (kval+1) % 4 );

            kit++;
            if (kit == key.end()) kit = key.begin();
        }

        // Set bits
        uchar c;
        int b = 0;

        for (int clr = 0; clr < 3; ++clr) {
            for (int i = 0; i < 4; ++i) {
                auto pos = find( ignore[clr].begin(), ignore[clr].end(), i );
                if (pos == ignore[clr].end()) {
                    setbit( c, getbit( (*mit)[clr], i), b++ );
                }
            }
        }

        if (c == 0) break;
        hash.push_back(c);
    }

    return hash;
}
