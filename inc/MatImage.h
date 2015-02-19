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

namespace Photocrypt
{

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

    long     cols()     const; ///< Returns the number of pixels in a row (width)
    long     rows()     const; ///< Returns the number of pixels in a column (height)
    long     step()     const; ///< Returns the number of bytes in a row
    short    channels() const; ///< Returns the number of bytes in a pixel
    short    bps()      const; ///< Returns the number of bits per sample
    uint8_t* data()     const; ///< Returns the pointer to the pixel array
    long     max()      const; ///< Returns the maximum size of text this image can hide
    bool     empty()    const; ///< Checks if image is empty

    /** Converts the image into a `Gdk::Pixbuf` object.
     *  
     *  This object can be used to display the image in a Gtk window.
     *
     *  @return A `Gdk::Pixbuf` object of the image */
    Glib::RefPtr<Gdk::Pixbuf> pixbuf() const;

    /** Scales an image to desired size.
     *
     *  It one parameter is set to 0, it is adjusted to keep aspect ratio.
     *
     *  @param width    The desired width of image
     *  @param height   The desired height of image
     *
     *  @return A scaled `Pixbuf` object
     */
    Glib::RefPtr<Gdk::Pixbuf> scale(int width, int height) const;

    /** Scales an image to fit in a desired size.
     *
     *  @param width    The width of the desired size
     *  @param height   The height of the desired size
     *  
     *  @return A scaled `Gdk::Pixbuf` object that fits in the given size
     */
    Glib::RefPtr<Gdk::Pixbuf> fit(int width, int height) const;

    /** Displays the image for a certain time.
     *
     *  @param  msecs   Number of milliseconds to display
     */
    void show(int msecs = 0) const;


    /** Hides a text in the image.
     *
     * It hides the text in the object itself without creating a new object.
     *
     * @param   text    The text to hide
     * @param   key     The password required to extract the hidden text
     *
     * @return  The object itself.
     */
    MatImage& steg(const std::string& text, const std::string& key);

    /** Extract the hidden text from the image.
     *
     * @param   key     The password used to extract the hidden text
     *
     * @return  The hidden text
     */
    std::string unsteg(const std::string& key) const;

private:
    cv::Mat mMat; ///< The OpenCV's image class that holds our data


    // Helpers

    /** Sets the key view the text hidden in the image */
    void set_key(const std::string& key);

    /** Conceals the given `text` in the image */
    void conceal(const std::string& text, const std::string& key);

    /** Returns the SHA1-hashed string of the key set for the image */
    std::string hash(const std::string& key) const;

    /** Returns the text hidden in the image */
    std::string reveal(const std::string& key) const;
};

} // namespace Photocrypt

#endif // MATIMAGE_H
