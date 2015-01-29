/** @file
 *  This file contains the decarations of the useful utility functions
 */
#ifndef UTIL_H
#define UTIL_H

#include <string>

/** All the utility functions are declared inside this namespace */
namespace util
{

/** Returns the SHA1-hashed string of the given string
 *
 * @param   in  The string to be hashed
 *
 * @return  The hashed hex string of @a in
 */
std::string sha(const std::string& in);


/** Sets a bit
 *
 * @param   p       A byte whose bits needs to be changed
 * @param   bit     The value to set (either 0 or 1)
 * @param   index   The position of bit to change
 */
void setbit(unsigned char& p, const int bit, const int index = 0);

/** Gets a bit
 *
 * @param p     A byte whose bits we want
 * @param index The position of bit we want
 *
 * @return  The value of the desired bit (either 0 or 1)
 */
int  getbit(const unsigned char& p, const int index = 0);


/** Converts a number to string
 *
 * @param number    A number to be converted
 *
 * @return  The @a number as a `std::string` object
 */
std::string to_str(double number);   

/** Displays an error message and quits program
 *
 * @param msg          The error message to display
 * @param return_value The value to return when quitting
 */
void error(const std::string& msg, int return_value = 1);

} // namespace util

#endif // UTIL_H
