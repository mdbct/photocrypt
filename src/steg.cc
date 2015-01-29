/** @file
 *  This file is a test program that tests the encryption of data (ie. the
 *  `steg()` method).
 *
 *  Also see the `unsteg.cc` file which tests the decryption part.
 */
#include <iostream>
#include "MatImage.h"
#include "TextFile.h"
#include <string>
#include "Error.h"
#include "util.h"
using namespace std;
using namespace util;

int main(int argc, char** argv)
{
    if (argc < 3 or argc > 4)
        error("USAGE: " + string(argv[0]) + " <carrier-image> <text-file> [<stego-image>]");

    // Get the string constants
    string image_filename = argv[1];
    string text_filename = argv[2];
    string stego_filename = (argc == 4)? argv[3] : "out.png";
    string key;

    // Open the files
    MatImage I = image_filename;
    TextFile file = TextFile::open(text_filename);

    // Ask for password
    cerr << "Enter a password: ";
    cin >> key;

    try
    {
        I.steg(file.str(), key).save(stego_filename);
        cout << "Stego-image saved as `" << stego_filename << "`." << endl;
    }
    catch (ImageEmptyError)
    {
        error("ERROR: Not a proper image.");
    }
    catch (TextEmptyError)
    {
        error("ERROR: The text is empty.");
    }
    catch (KeyEmptyError)
    {
        error("ERROR: Password is empty.");
    }
    catch (InsufficientImageError)
    {
        error("ERROR: Image is not large enough to hide the text.");
    }
}
