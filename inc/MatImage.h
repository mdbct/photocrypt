/** @file
 *  This file contains the declaration of the `MatImage` class that represents
 *  an image.
 *
 *  It provides basic methods to manipulate images and hide text inside them.
 */
#ifndef MATIMAGE_H
#define MATIMAGE_H

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <gtkmm.h>

/**
 *  The class that represents an image that can hide text within it.
 */
class MatImage
{
    public:
        /** Creates an empty image with nothing */
        MatImage();

        /** Creates an `MatImage` object from an image file */
        MatImage(const std::string& filename);

        /** Creates an `MatImage` object from a `Pixbuf` object */
        MatImage(const Glib::RefPtr<Gdk::Pixbuf>& p);

        /** Creates an `MatImage` object from a `Mat` object */
        MatImage(const cv::Mat& mat);

        /** Copies another `MatImage` object */
        MatImage(const MatImage& image);

        /** Destructor, does nothing */
        virtual ~MatImage();


        /** Saves the image to a file with the given filename */
        void save(const std::string& filename) const;


        // Getters

        long cols() const;  ///< Returns the number of pixels in a row (width)
        long rows() const;  ///< Returns the number of pixels in a column (height)
        long step() const;  ///< Returns the number of bytes in a row
        short channels() const; ///< Returns the number of bytes in a pixel
        short bps() const;  ///< Returns the number of bits per sample
        uint8_t* data() const;  ///< Returns the pointer to the pixel array
        long max() const;   ///< Returns the maximum size of text this image can hide
        bool empty() const; ///< Checks if image is empty
        bool is_stego() const; ///< Checks if image is a stego-image

        /** Returns a Pixbuf object to be used in gtkmm */
        Glib::RefPtr<Gdk::Pixbuf> pixbuf() const;

        /** Returns a scaled `MatImage` object.
         *
         *  It one parameter is set to 0, it is adjusted to keep aspect ratio.
         */
        Glib::RefPtr<Gdk::Pixbuf> scale(int width, int height) const;

        /** Returns a scaled `MatImage` object that fits in the given dimensions */
        Glib::RefPtr<Gdk::Pixbuf> fit(int width, int height) const;

        /** Displays the image for given milliseconds */
        void show(int msecs = 0) const;


        /** Hides the given `text` in the image with the given`key` */
        MatImage& steg(const std::string& text, const std::string& key);

        /** Returns the text hidden in the image with the given `key` */
        std::string unsteg(const std::string& key) const;

    private:
        cv::Mat mMat; ///< The OpenCV's image class that holds our data


        // Helpers
        
        /** Sets the key view the text hidden in the image */
        void set_key(const std::string& key);
        
        /** Conceals the given `text` in the image */
        void conceal(const std::string& text);

        /** Returns the SHA1-hashed string of the key set for the image */
        std::string hash() const;

        /** Returns the text hidden in the image */
        std::string reveal() const;
};

#endif // MATIMAGE_H
