/** @file
 *  This file contains the definition of the `TextFile` class which was
 *  declared in `TextFile.h` file.
 */
#include "TextFile.h"
#include <fstream>

using namespace std;

// Constructor
TextFile::TextFile(const string& str) : mText(str) { }

// Destructor
TextFile::~TextFile() { }

// Open a file
TextFile TextFile::open(const string& filename)
{
    TextFile t;

    ifstream file(filename.c_str());

    char c;
    while (file.get(c))
        t.mText.push_back(c);

    return t;
}

// Size of file
string::size_type TextFile::size() const
{
    return mText.size();
}

// Returns as a std::string object
string TextFile::str() const
{
    return mText;
}

// Convert to std::string
TextFile::operator std::string() const
{
    return mText;
}

// Save to disk
void TextFile::save(const string& filename)
{
    ofstream file(filename.c_str());

    string::iterator sit;
    for (sit = mText.begin(); sit != mText.end(); ++sit)
        file.put(*sit);
}

// `<<` overloading
ostream& operator<<(ostream& out, const TextFile& textFile)
{
    return (out << textFile.str());
}

// Append
TextFile& TextFile::append(const string& str)
{
    mText += str;
    return *this;
}

TextFile& TextFile::operator+=(const string& str)
{
    return this->append(str);
}

TextFile& TextFile::append(const TextFile& textFile)
{
    return this->append(textFile.str());
}

TextFile& TextFile::operator+=(const TextFile& textFile)
{
    return this->append(textFile.str());
}
