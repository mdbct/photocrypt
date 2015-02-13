# Photocrypt

A [steganography][wiki-steganography] program that hides text in image. It has
both a graphical ([gtkmm][gtkmm]) and a command-line interface. It is written
in C++.

It implements the Least Significant Bit (LSB) algorithm with password
protection. Every bit of secret data is stored in the LSBs of the image pixel
values. This makes the stego-image practically identical to the
original image. The algorithm depends on the key so it is unfeasible to extract
data without the key.

The algorithm used in this program can be explained briefly as:

* The 1st row of image is used to store the 40-byte SHA1 hash string of the
  password. The bits used to hide the data depends on the key itself. This is
  required to add password-verification feature.

* The secret data starts from the beginning of the 2nd row. The bits used to
  hide the data depends on the key itself. The value `\0` is appened at last to
  mark the end of text.

<blockquote>
<strong>Note:</strong> You cannot extract text from a JPEG image because the
JPEG compression algorithm modifies some pixels to reduce file size and quality.
</blockquote>

The CLI programs writes the stego-image in `out.png` file in the current
directory, and writes the hidden text in `stdout` (which can be redirected).


## Dependencies

It depends on following packages only to build. Built package is independent.

| Package                    | Task                              |
|----------------------------|-----------------------------------|
| [GNU gcc][gcc] with C++11  | To compile the sources            |
| [GNU make][make]           | To build                          |
| [pkg-config][pkg-config]   | To get compiler and linker flags  |
| [OpenCV][opencv] 2 library | For image processing              |
| [gtkmm][gtkmm] 2 library   | For GUI                           |
| [OpenSSL][openssl] library | For security                      |



## Installation

### [Arch Linux][archlinux]

[Photocrypt][aur-photocrypt] is available for [Arch Linux][archlinux]
via [AUR][aur]:

    yaourt photocrypt-git

### Manual installation

To install to system:

    sudo make install

To list available make targets:

    make help

## Usage

To open the GUI frontend, run:

    photocrypt

Or if you are a terminal-lover like me, here's how you can hide a text file
`secret.txt` in an image `image.jpg`:

    steg image.jpg secret.txt

Similarly, to view the text hidden in a stego-image `stego.png` (You can then
redirect the output to file yourself):

    unsteg stego.png


[wiki-steganography]: http://en.wikipedia.org/wiki/Steganography
[gtkmm]:              http://gtkmm.org
[aur]:                https://aur.archlinux.org
[aur-photocrypt]:     https://aur.archlinux.org/packages/photocrypt-git
[archlinux]:          https://archlinux.org
[gcc]:                http://gcc.gnu.org
[make]:               http://gnu.org/software/make
[pkg-config]:         http://pkgconfig.freedesktop.org/wiki
[opencv]:             http://opencv.org
[openssl]:            http://openssl.org
