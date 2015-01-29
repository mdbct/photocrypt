/** @file
 *  This file contains the decarations of the useful utility functions
 */
#ifndef UTIL_H
#define UTIL_H

#include <string>

/** All the utility functions are declared inside this namespace */
namespace util
{

/** Returns the SHA1-hashed string of the given string */
std::string sha(const std::string& in);


/** Sets a bit */
void setbit(unsigned char& p, const int bit, const int index = 0);

/** Gets a bit */
int  getbit(const unsigned char& p, const int index = 0);


/** Converts a real number to string */
std::string to_str(double number);   

} // namespace util

#endif // UTIL_H
