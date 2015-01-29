/** @file
 *  This file defines the utlity function declared in `util.h` file.
 */
#include "util.h"
#include <sstream>
#include <openssl/sha.h>
#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>

typedef unsigned char uchar;

using namespace std;

// Returns a SHA1-hashed string of the given string.
string util::sha(const string& in)
{
    uchar temp[SHA_DIGEST_LENGTH]; // SHA_DIGEST_LENGTH = 20 bytes

    // This is the library function, declared in <openssl/sha.h>
    // See 'man 3 sha' for details.
    SHA1((uchar*) in.c_str(), in.size(), temp);

    // We'll be writing the output string to a stringstream;
    ostringstream o;

    // Convert every byte into HEX string and add to o.
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        // If the string is single digit, add a "0" before it.
        if (temp[i] <= 0xF)
            o << "0";

        o << hex << (int) temp[i];
    }

    assert(o.str().size() == 2 * SHA_DIGEST_LENGTH);
    return o.str();
}


// Sets a bit.
void util::setbit(uchar& p, const int bit, const int index)
{
    assert(bit == 0 or bit == 1);
    assert(index >= 0 and index < 8);

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
int util::getbit(const uchar& p, const int index)
{
    int isSet = p & (1 << index);

    return (isSet) ? 1 : 0;
}


// Convert a number to string
string util::to_str(double number)
{
    ostringstream out;
    out << number;
    return out.str();
}
