/** @file:
 *
 *  This file declares some exceptions used in this project.
 *
 *  It also declares some functions related to error handling.
 */
#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept>
#include <exception>

/** A base class for exceptions in our project */
struct Error : public std::runtime_error
{
    /** Constructs an `Error` object with a given message */
    Error(const std::string& msg = "There has been an error");
};

/** The exception thrown when one tries to manipulate empty `MatImage` */
struct ImageEmptyError : public Error
{
    ImageEmptyError(const std::string& msg = "Image is empty");
};

/** The exception thrown when one tries to steg an empty string in an image */
struct TextEmptyError : public Error
{
    TextEmptyError(const std::string& msg = "Text is empty");
};

/** The exception thrown when one tries to set an empty password */
struct KeyEmptyError : public Error
{
    KeyEmptyError(const std::string& msg = "Key is empty");
};

/** The exception thrown when the image is not large enough */
struct InsufficientImageError : public Error
{
    InsufficientImageError(const std::string& msg = "Image is not large enough");
};

/** The exception thrown when one tries to unsteg with incorrect password */
struct KeyMismatchError : public Error
{
    KeyMismatchError(const std::string& msg = "Password is incorrect");
};

/** The exception thrown due to I/O error */
struct IOError : public Error
{
    IOError(const std::string& msg = "Input/Output error");
};

/** Displays an error message and aborts the program
 *  
 * @param error_msg    The error message
 * @param ret          The return value
 */
void error(const std::string& error_msg = "There has been an error", int ret = -1);

/** Displays an error message of the exception and aborts
 *
 * @param e     The exception thrown
 * @param ret   The return value
 */
void error(const std::exception& e, int ret = -1);

#endif // ERROR_H
