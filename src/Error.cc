/** @file
 *
 *  This file defines the errors used in this project
 */

#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include "Error.h"
using namespace std;

Error::Error(const string& msg) : runtime_error(msg) {}

ImageEmptyError::ImageEmptyError(const string& msg) : Error(msg) {}

TextEmptyError::TextEmptyError(const string& msg) : Error(msg) {}

KeyEmptyError::KeyEmptyError(const string& msg) : Error(msg) {}

InsufficientImageError::InsufficientImageError(const string& msg) : Error(msg) {}

KeyMismatchError::KeyMismatchError(const string& msg) : Error(msg) {}

IOError::IOError(const string& msg) : Error(msg) {}


// Error with message
void error(const string& error_msg, int ret)
{
    cerr << ":: ERROR ::  " << error_msg << endl;
    exit(ret);
}


// Error with exception
void error(const exception& e, int ret)
{
    error(e.what(), ret);
}
