/**
 * Some global informations about the program.
 */
#include <vector>
#include <gtkmm.h>

const auto PROGRAM_TITLE   = "Photocrypt";
const auto PROGRAM_VERSION = "1.0";
const auto PROGRAM_AUTHORS = std::vector<Glib::ustring>({
        "Manish Munikar",
        "Dipesh Pandey",
        "Brihat Ratna Bajracharya"
        });

const auto PROGRAM_WEBSITE       = "https://github.com/mdbct/photocrypt";
const auto PROGRAM_WEBSITE_LABEL = "@ Github";
const auto PROGRAM_DESCRIPTION   = "A simple steganography application.";
const auto PROGRAM_LICENSE_TYPE  = Gtk::LICENSE_GPL_3_0;
const auto PROGRAM_COPYRIGHT     = "© MDBCT. 2015";
