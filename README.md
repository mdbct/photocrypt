photocrypt
==========

A steganography program that hides text in images.

It implements the Least Significant Bit (LSB) algorithm with SHA1 password
protection. Every bit of secret data is stored in the LSBs of the image pixel
values. This makes the stego-image practically identical to the original
image.

The algorithm used in this program can be explained briefly as:

* The 1st row of image is used to store the 40-byte SHA1 hash string of the
  password. Each bit of hash string is stored in the LSB of each byte of the
  image.

* The secret data starts from the beginning of the 2nd row. Only LSBs of
  the pixels are changed. Other 7 more significant bits are left as is.

* The end of text is marked by a pixel whose blue-coefficient is zero. Blue,
  because OpenCV stores images in BGR format instead of RGB. So if a
  blue-coefficient is found to be already zero before reaching the end of
  text, its 2nd LSB is set to 1, to prevent the decoding algorithm to think
  that it's the end of text.

At the moment, it outputs stego-image named 'out.png' by default. So, you have
to rename and move the file yourself. Also, the decrypting program outputs the
hidden text on the `stdout` by default. You can redirect it to a file instead.

<blockquote>
<strong>Note:</strong> You cannot extract text from a JPEG image because the JPEG
compression algorithm deletes some pixels to reduce file size and quality.
</blockquote>


Requirements
------------

- GNU gcc
- GNU make
- OpenCV 2 C++ library
- `pkg-config` package

Compilation
-----------

To build the program:

    make

To unbuild:

    make clean

For help on available make targets:

    make help

Usage
-----

To hide a text file `hello.txt` in an image `image.jpg`:

    ./steg image.jpg hello.txt

To view the text hidden in the stego-image `out.bmp`:

    ./unsteg out.png

You can save the extracted text in a file by using redirection:

    ./unsteg out.png > new_file.txt
