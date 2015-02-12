# Photocrypt

A [steganography](http://en.wikipedia.org/wiki/Steganography) program that
hides text in images. It has both a graphical ([gtkmm](http://gtkmm.org))
and a command-line interface. It is written in C++.

It implements the Least Significant Bit (LSB) algorithm with SHA1 password
protection. Every bit of secret data is stored in the LSBs of the image pixel
values. The algorithm depends on the key so it is unfeasible to extract data
without the key. This makes the stego-image practically identical to the
original image.

The algorithm used in this program can be explained briefly as:

* The 1st row of image is used to store the 40-byte SHA1 hash string of the
  password. The bits used to hide the data depends on the key itself.

* The secret data starts from the beginning of the 2nd row. The bits used to
  hide the data depends on the key itself. The value `\0` is appened at last to
  mark the end of text.

<blockquote>
<strong>Note:</strong> You cannot extract text from a JPEG image because the
JPEG compression algorithm deletes some pixels to reduce file size and quality.
</blockquote>

The CLI programs writes the stego-image in `out.png` file in the current
directory, and writes the hidden text in `stdout`.


## Dependencies

- [GNU gcc](http://gcc.gnu.org): to compile
- [GNU make](http://www.gnu.org/software/make): to build
- [pkg-config](http://pkgconfig.freedesktop.org/wiki/): to link
- [OpenCV](http://opencv.org) 2 library: for image processing
- [gtkmm](http://gtkmm.org) 2.4 library: for GUI
- [OpenSSL](http://openssl.org) library: for security


## Installation

#### Arch Linux

[Photocrypt](https://aur.archlinux.org/packages/photocrypt-git)
is available for [Arch Linux](https://archlinux.org) via
[AUR](https://aur.archlinux.org):

```bash
yaourt photocrypt-git
```

### Manual installation

To compile the program:

```bash
make [all]
```

To install to system:

```bash
sudo make install
```

To get the list of available make targets:

```bash
make help
```

## Usage

To open the GUI frontend, run:

```bash
photocrypt
```

Or if you are a terminal-lover like me, here's how you can hide a text file
`secret.txt` in an image `image.jpg`:

```bash
steg image.jpg secret.txt
```

Similarly, to view the text hidden in a stego-image `stego.png` (You can then
redirect the output to file yourself):

```bash
unsteg stego.png
```

