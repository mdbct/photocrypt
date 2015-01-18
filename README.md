photocrypt
==========

A program that hides text in images.

Right now, the program is **very** simple. All it can do is hide
text files of maximum 2550 bytes in any image. It produces a stego-image
with the text hidden in it. The produced image is grayscale (B&W) because
pixel manipulation is much more easier in grayscale images. It will be made
to support colored images in future though.

Also, I didn't use the LSB algorithm. The algorithm I used is even simpler,
but we can see the difference in the image. :p

The algorithm I used is:

    1.  Reserve the first 10 pixels of the image to store the size of the text
        file such that:

            total size = sum of the first ten pixel values

        Each pixel can store values from 0-255. So maximum file size allowed is
        2550 bytes.

    2.  for (i = 0; i < text.size(); ++i)
            pixel[11 + i] = text[i]

    3.  Done! :D


Requirements
------------

- OpenCV 2 C++ library
- GNU make
- GNU g++
- `pkg-config` package

Compilation
-----------

    make

Usage
-----

To hide a text file `hello.txt` in an image `image.jpg`:

    ./steg image.jpg hello.txt

To view the text hidden in the stego-image `stego.bmp`:

    ./unsteg stego.bmp
