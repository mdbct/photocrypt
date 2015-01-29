/** @file
 *
 *  This file defines the errors used in this project
 */

#include "Error.h"

using std::string;

Error::Error(const string& msg) : mMsg(msg) {}

string Error::what() const { return mMsg; }


ImageEmptyError::ImageEmptyError(const string& msg)               : Error(msg) {}
TextEmptyError::TextEmptyError(const string& msg)                 : Error(msg) {}
KeyEmptyError::KeyEmptyError(const string& msg)                   : Error(msg) {}
InsufficientImageError::InsufficientImageError(const string& msg) : Error(msg) {}
ImageNotStegoError::ImageNotStegoError(const string& msg)         : Error(msg) {}
KeyMismatchError::KeyMismatchError(const string& msg)             : Error(msg) {}
