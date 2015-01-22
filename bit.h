/*
 * This file contains declarations of some useful functions that can
 * access every binary bit in memory.
 */
#pragma once

// sets a bit of `p` to `bit` at the position given by `index`
bool set_bit(unsigned char& p, const unsigned bit, const unsigned index = 0);

// gets a bit of `p` at position given by `index`
unsigned get_bit(const unsigned char& p, const unsigned index);
