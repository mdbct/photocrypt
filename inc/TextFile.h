/** @file
 *  This file declares a class that represents a text file buffer.
 */
#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <string>
#include <iostream>


/**
 *  This class represents a text file buffer.
 *
 *  It facilitates us with basic and common text file operations like
 *  opening, saving, appending, displaying, etc.
 */
class TextFile
{
public:
    /** Initializes a text file buffer with the given string */
    TextFile(const std::string& str = "");

    /** Destructor, does nothing */
    virtual ~TextFile();

    /** Opens a text file and returns a `TextFile` object */
    static TextFile open(const std::string& filename);

    /** Returns the size (in bytes) of the text file */
    std::string::size_type size() const;

    /** Returns the contents of the text file as a `std::string` object */
    std::string str() const;

    /** Convert into std::string */
    operator std::string() const;

    /** Overloads the `<<` operator to be used for I/O */
    friend std::ostream& operator<<(std::ostream&, const TextFile&);

    /** Appends the given string to the text file */
    TextFile& append(const std::string& str);
    /** Appends the given string to the text file */
    TextFile& operator+=(const std::string& str);

    /** Appends the given text file to the text file */
    TextFile& append(const TextFile& textFile);
    /** Appends the given text file to the text file */
    TextFile& operator+=(const TextFile& textFile);

    /** Saves the text file to the disk with the given filename */
    void save(const std::string& filename);

private:
    std::string mText; /**< Holds the contents of the text file */
};


#endif // TEXTFILE_H
