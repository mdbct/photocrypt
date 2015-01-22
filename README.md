photocrypt
==========

A steganography program that hides text in images.

It implements the Least Significant Bit (LSB) algorithm with SHA1 password
protection. Every bit of secret data is stored in the LSBs of the image pixel
values. This makes the stego-image practically identical to the original
image.

The algorithm used in this program can be explained briefly as:

* The 1st row of image is used to store the 40-byte SHA1 hex string of the
  password.

* The secret data starts from the beginning of the 2nd row. Only LSBs of
  the pixels are changed. Other 7 more significant bits are left as is.

* The end of text is marked by a pixel whose blue-coefficient is zero. Blue,
  because OpenCV stores images in BGR format instead of RGB. So if a
  blue-coefficient is found to be already zero before reaching the end of
  text, its 2nd LSB is set to 1, to prevent the decoding algorithm to think
  that it's the end of text.

At the moment, it outputs stego-image named 'out.bmp' by default. So, you have
to rename and move the file yourself.


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
