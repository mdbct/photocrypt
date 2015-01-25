/** 
 *  This file contains the declaration of the `Image` class that represents
 *  an image.
 *
 *  It provides basic methods to manipulate images and hide text inside them.
 */
#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

/**
 *  The class that represents an image that can hide text within it.
 */
class Image
{
    public:
        /** Creates an empty image with nothing */
        Image();

        /** Copies another `Image` object */
        Image(const Image& image);

        /** Destructor, does nothing */
        virtual ~Image();



        /** Opens an image file and returns its `Image` object */
        static Image open(const std::string& filename);

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


        /** Displays the image for given milliseconds */
        void show(int msecs = 0) const;


        /** Hides the given `text` in the image with the given`key` */
        Image& steg(const std::string& text, const std::string& key);

        /** Returns the text hidden in the image with the given `key` */
        std::string unsteg(const std::string& key) const;

        /** Sets the key view the text hidden in the image */
        void set_key(const std::string& key);

    private:
        cv::Mat mMat; ///< The OpenCV's image class that holds our data


        // Helpers
        
        /** Conceals the given `text` in the image */
        void conceal(const std::string& text);

        /** Returns the text hidden in the image */
        std::string reveal() const;

        /** Returns the SHA1-hashed string of the key set for the image */
        std::string hash() const;
};

#endif // IMAGE_H
