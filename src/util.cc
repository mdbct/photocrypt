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

// Returns a SHA1 digest of the given string.
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
    /*
    // We'll be writing the output string to a stringstream;
    ostringstream o;

    // Convert every byte into HEX string and add to o.
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        // If the string is single digit, add a "0" before it.
        if (temp[i] <= 0xF)
            o << "0";

        o << hex << static_cast<int>(temp[i]);
    }

    return o.str();
    */
}


// Sets a bit.
void setbit(uchar& p, const int bit, const int index)
{
    if (bit < 0 or bit > 1)
        throw out_of_range("Bit should be either 0 or 1");

    if (index < 0 or index > 7)
        throw out_of_range("Index is out of range while setting bit");

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

// Get a bit.
int getbit(const uchar& p, const int index)
{
    if (index < 0 or index > 7)
        throw out_of_range("Index is out of range while getting bit");

    int isSet = p & (1 << index);

    return (isSet) ? 1 : 0;
}

} // namespace Photocrypt
