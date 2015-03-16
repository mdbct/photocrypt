# Photocrypt

A [steganography][wiki-steganography] program that hides text in image. It has
both a graphical ([gtkmm][gtkmm]) and a command-line interface. It is a
cross-platform application written in C++.

It implements a modified LSB (least significant bit) substitution algorithm
with password protection. Every bit of secret data is stored in the *less*
significant bits (not always *least*) of the image pixel color values. This
makes the stego-image practically identical to the original image. The
algorithm depends on the key so it is unfeasible to extract data without the
key.

The algorithm used in this program can be explained briefly as:

* The 1st row of image is used to store the SHA1 digest of the password. The
  bits used to hide the data depends on the key itself. This is required to add
  password-verification feature.

* The secret data starts from the beginning of the 2nd row. The bits used to
  hide the data depends on the key itself. The `\0` character is appended at
  last to mark the end of text.

<blockquote>
<strong>Note:</strong> You cannot extract text from a JPEG image because the
JPEG compression algorithm modifies some pixels to reduce file size and quality.
</blockquote>


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

    make
    sudo make install

To list available make targets:

    make help

## Usage

To open the GUI front-end, run:

    photocrypt

But if you're a terminal-person like me, you can use the CLI front-ends (`steg`
and `unsteg`) which are equally (probably more?) powerful. For example, to hide
the contents of `text.txt` in `image.jpg`:

    steg image.jpg -f text.txt

Similarly, to decrypt the text hidden in `stego.png`:

    unsteg stego.png

For more info, see `steg -h` and `unsteg -h`.

## Authors

* Manish Munikar, `070-BCT-520`
* Dipesh Pandey, `070-BCT-514`
* Brihat Ratna Bajracharya, `070-BCT-513`

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
