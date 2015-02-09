# photocrypt

A [steganography](http://en.wikipedia.org/wiki/Steganography) program that
hides text in images. It has both a graphical ([gtkmm](http://gtkmm.org))
and a command-line interface. It is written in C++.

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

<blockquote>
<strong>Note:</strong> You cannot extract text from a JPEG image because the
JPEG compression algorithm deletes some pixels to reduce file size and quality.
</blockquote>

The CLI programs writes the stego-image in `out.png` file in the current
directory, and writes the hidden text in `stdout`.


## Requirements

- [GNU gcc](http://gcc.gnu.org)
- [GNU make](http://www.gnu.org/software/make)
- [pkg-config](http://pkgconfig.freedesktop.org/wiki/) package
- [OpenCV](http://opencv.org) 2 library
- [gtkmm](http://gtkmm.org) 2.4 library
- [OpenSSL](http://openssl.org) library


## Installation

#### Arch Linux

[Photocrypt](https://aur.archlinux.org/packages/photocrypt-git)
is available for [Arch Linux](https://archlinux.org) via
[AUR](https://aur.archlinux.org):

```bash
yaourt photocrypt-git
```

### Manual Compilation

To build the GUI program:

```bash
make
```

To build the CLI program:

```bash
make cli
```

To clean:

```bash
make clean
```

To list available `make` targets:

```bash
make help
```


## Usage

To open the GUI frontend, run:

```bash
./photocrypt
```

Or if you are a terminal-lover like me, here's how you can hide a text file
`secret.txt` in an image `image.jpg`:

```bash
./steg image.jpg secret.txt
```

Similarly, to view the text hidden in a stego-image `stego.png`:

```bash
./unsteg stego.png
```
