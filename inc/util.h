/** @file
 *
 *  This file contains the decarations of the useful utility functions
 */
#ifndef UTIL_H
#define UTIL_H

#include <string>

/** Namespace for the "Photocrypt" project */
namespace Photocrypt
{

/** Returns the SHA1-hashed string of the given string.
 *
 * The return string is always 20 bytes long.
 *
 * @param   in  The string to be hashed
 *
 * @return  The hashed string of @a in
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

} // namespace Photocrypt

#endif // UTIL_H
