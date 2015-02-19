/** @file
 *
 * This is the photocrypt decrypter.
 */
#include <iostream>
#include <string>
#include <getopt.h>
#include "MatImage.h"
#include "TextFile.h"
#include "Error.h"
#include "INFO.h"
using namespace std;
using namespace Photocrypt;

// Global variables
const string PROGRAM = "unsteg";

// Helper functions
void print_help();
void print_version();

//==================== main() ===================================================
int main(int argc, char** argv)
{
    // Declare necessary strings
    string image_filename;
    string text;
    string key;
    string out_filename;

    // Parse command-line options
    int option_index = 0;
    option long_options[] = {
        {"help",      no_argument,       0, 'h'},
        {"password",  required_argument, 0, 'p'},
        {"out-file",  required_argument, 0, 'o'},
        {"version",   no_argument,       0, 'V'},
        {0,0,0,0}
    };

    while (true) {
        int c = getopt_long(argc, argv, ":hVp:o:", long_options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 'h':
                print_help();
                return 0;

            case 'V':
                print_version();
                return 0;

            case 'p':
                key = string(optarg);
                break;

            case 'o':
                out_filename = string(optarg);
                break;

            case ':':
                error("Missing option argument");

            case '?':
                error("Unknown option");

            default:
                error("Unknown error");
        }
    }

    if ( (argc - optind) != 1 ) {
        print_help();
        return 1;
    }

    // Open the image
    image_filename = string(argv[optind++]);
    MatImage I;
    try
    {
        I = image_filename;
    }
    catch (const IOError& e)
    {
        error(e);
    }

    // Get the password
    if (key.empty()) {
        cout << ":: Password: ";
        getline(cin, key);
    }

    // Unsteg
    TextFile file;
    try
    {
        file = I.unsteg(key);

        if (out_filename.empty()) {
            cout << file << endl;
        } else {
            file.save(out_filename);
            cout << ":: Hidden text extracted to '" << out_filename <<"'" << endl;
        }
    }
    catch (const exception& e)
    {
        error(e);
    }

    return 0;
}

//======================== print_version() ====================================
void print_version()
{
    cout << PROGRAM_VERSION << endl;
}

//======================== print_help() =======================================
void print_help()
{
    cout << PROGRAM << " " << PROGRAM_VERSION << endl;
    cout << endl <<
        PROGRAM_DESCRIPTION << " (decrypter)" << endl;

    cout << "\n"
        "USAGE:\n"
        "  " << PROGRAM << " [OPTION...] IMAGE-FILE\n";

    cout << "\n"
        "OPTIONS:\n"
        "  -h, --help               display this help message\n"
        "  -p, --password=PASSWD    password to decrypt\n"
        "  -o, --out-file=FILE      output filename\n"
        "  -V, --version            display the program version number\n";

    cout << "\n"
        "AUTHORS:\n";
    for (auto author : PROGRAM_AUTHORS)
        cout << "  " << author << endl;

    cout << endl <<
        "This tool is a part of the Photocrypt project\n" <<
        PROGRAM_WEBSITE << endl <<
        PROGRAM_COPYRIGHT << endl;
}
