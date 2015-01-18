photocrypt
==========

A program that hides text in images.

Right now, the program is **very** simple. All it can do is hide
text files of maximum 2550 bytes in any image. It produces a stego-image
with the text hidden in it. The produced image is grayscale (B&W) because
pixel manipulation is much more easier in grayscale images. It will be made
to support colored images in future though.

Requirements
------------

- OpenCV 2 C++ library
- GNU make
- GNU gcc

Compilation
-----------

    make

Usage
-----

To hide a text file `hello.txt` in an image `image.jpg`:

    ./steg image.jpg hello.txt

To view the text hidden in the stego-image `stego.bmp`:

    ./unsteg stego.bmp
