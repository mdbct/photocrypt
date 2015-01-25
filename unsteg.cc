/** @file
 *  This file is a simple test program that tests the decryption of data
 *  from a stego-image. (ie. it tests the `unsteg()` method.)
 *
 *  Also, see the `steg.cc` file, which tests the encryption part.
 */
#include <iostream>
#include <string>
#include "Image.h"
#include <cassert>

using namespace std;

int main(int argc, char** argv)
{
    // Must pass 1 argument
    assert(argc == 2);

    // Get the filename
    string stego_filename = argv[1];
    string key;

    // Open the stego-image
    Image I = Image::open(stego_filename);

    // Ask for password:
    cerr << "Enter password: ";
    cin >> key;

    // Display the hidden text
    cout << I.unsteg(key) << endl;
}
