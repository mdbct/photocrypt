/** @file:
 *
 *  This file declares some exceptions used in this project
 */

#include <string>

class Error
{
    public:
        Error(const std::string& msg = "");
        std::string what() const;
    private:
        std::string mMsg;
};

struct ImageEmptyError : public Error
{
    ImageEmptyError(const std::string& msg = "");
};

struct TextEmptyError : public Error
{
    TextEmptyError(const std::string& msg = "");
};

struct KeyEmptyError : public Error
{
    KeyEmptyError(const std::string& msg = "");
};

struct InsufficientImageError : public Error
{
    InsufficientImageError(const std::string& msg = "");
};

struct ImageNotStegoError : public Error
{
    ImageNotStegoError(const std::string& msg = "");
};

struct KeyMismatchError : public Error
{
    KeyMismatchError(const std::string& msg = "");
};
