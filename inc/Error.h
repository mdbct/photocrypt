/** @file:
 *
 *  This file declares some exceptions used in this project
 */

#include <string>

/** A base class for exceptions in our project */
class Error
{
    public:
        /** Constructs an `Error` object with a given message */
        Error(const std::string& msg = "");

        /** Returns the message of the exception */
        std::string what() const;

    private:
        /** The message of the exception */
        std::string mMsg;
};

/** The exception thrown when one tries to manipulate empty `MatImage` */
struct ImageEmptyError : public Error
{
    ImageEmptyError(const std::string& msg = "");
};

/** The exception thrown when one tries to steg an empty string in an image */
struct TextEmptyError : public Error
{
    TextEmptyError(const std::string& msg = "");
};

/** The exception thrown when one tries to set an empty password */
struct KeyEmptyError : public Error
{
    KeyEmptyError(const std::string& msg = "");
};

/** The exception thrown when the image is not large enough */
struct InsufficientImageError : public Error
{
    InsufficientImageError(const std::string& msg = "");
};

/** The exception thrown when one tries to unsteg a non-stego image */
struct ImageNotStegoError : public Error
{
    ImageNotStegoError(const std::string& msg = "");
};

/** The exception thrown when one tries to unsteg with incorrect password */
struct KeyMismatchError : public Error
{
    KeyMismatchError(const std::string& msg = "");
};
