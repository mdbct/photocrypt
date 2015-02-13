/** @file
 *  This file is a simple test program that tests the decryption of data
 *  from a stego-image. (ie. it tests the `unsteg()` method.)
 *
 *  Also, see the `steg.cc` file, which tests the encryption part.
 */
#include <iostream>
#include <string>
#include "MatImage.h"
#include <cassert>
#include "util.h"
#include "Error.h"

using namespace std;
using namespace util;

int main(int argc, char** argv)
{
    if (argc != 2)
        error("USAGE: " + string(argv[0]) + " <stego-image>");

    // Get the string constants
    string stego_filename = argv[1];
    string key;

    // Open the files
    MatImage I = stego_filename;

    // Ask for password
    cerr << "Enter the password: ";
    cin >> key;

    string text;
    try
    {
        text = I.unsteg(key);
    }
    catch (ImageEmptyError)
    {
        error("ERROR: Not a proper image.");
    }
    catch (KeyEmptyError)
    {
        error("ERROR: Password is empty.");
    }
    catch (KeyMismatchError)
    {
        error("ERROR: Password incorrect.");
    }

    cout << text << endl;
}
