/**
 *  This file is a test program that tests the encryption of data (ie. the
 *  `steg()` method).
 *
 *  Also see the `unsteg.cc` file which tests the decryption part.
 */
#include <iostream>
#include "Image.h"
#include "TextFile.h"
#include <string>
#include <cassert>

using namespace std;

int main(int argc, char** argv)
{
    // Must pass 2 arguments
    assert(argc == 3);

    // Set the filenames in strings
    string image_filename = argv[1];
    string text_filename = argv[2];
    string stego_filename = "out.png";
    string key;

    // Open the image and the text file
    Image I = Image::open(image_filename);
    TextFile file = TextFile::open(text_filename);

    // Display useful information
    cout << "Capacity:  " << I.max() << endl;
    cout << "Text size: " << file.size() << endl;

    // Text size should be less than the maximum capacity of the image
    assert(file.size() < I.max());

    // Ask to enter a password
    cout << "Enter a password: ";
    cin >> key;

    // Steg the shit
    I.steg(file.str(), key).save(stego_filename);
    cout << "Stego image written to " + stego_filename << endl;
}
