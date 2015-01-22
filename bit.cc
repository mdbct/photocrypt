#include "bit.h"
#include <cassert>

typedef unsigned char   uchar;
typedef unsigned int    uint;

using namespace std;

bool set_bit(uchar& p, const uint bit, const uint index)
{
    assert(index < 8);

    bool success = true;

    switch (bit)
    {
        case 1:
            p |= (1 << index);
            break;

        case 0:
            p &= ~(1 << index);
            break;

        default:
            success = false;
            break;
    }

    return success;
}

uint get_bit(const uchar& p, const uint index)
{
    bool is_set = (p & (1 << index));

    return (is_set) ? 1 : 0;
}
