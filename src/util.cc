/** @file
 *
 *  This file defines the utlity function declared in `util.h` file.
 */
#include "util.h"
#include <sstream>
#include <openssl/sha.h>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <stdexcept>
using namespace std;

using uchar = unsigned char;

namespace Photocrypt
{

// Returns a SHA1 digest of the given string
string sha(const string& in)
{
    uchar temp[SHA_DIGEST_LENGTH]; // SHA_DIGEST_LENGTH = 20 bytes

    // This is the library function, declared in <openssl/sha.h>
    // See 'man 3 sha' for details.
    SHA1((uchar*) in.c_str(), in.size(), temp);

    string s;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        s.push_back(temp[i]);
    return s;
}


// Sets a bit
void setbit(uchar& p, const int bit, const int index)
{
    // Check for pre-conditions
    if (bit < 0 or bit > 1)
        throw out_of_range("Bit should be either 0 or 1");

    if (index < 0 or index > 7)
        throw out_of_range("Index is out of range while setting bit");

    // Set the bit
    switch (bit)
    {
    case 1:
        p |= (1 << index);
        break;

    case 0:
        p &= ~(1 << index);
        break;

    default:
        exit(1);
    }
}

// Get a bit
int getbit(const uchar& p, const int index)
{
    // Check for pre-conditions
    if (index < 0 or index > 7)
        throw out_of_range("Index is out of range while getting bit");

    int isSet = p & (1 << index);

    return (isSet) ? 1 : 0;
}

} // namespace Photocrypt
