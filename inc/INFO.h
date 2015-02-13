/** @file
 *  Some global informations about the program.
 */
#ifndef INFO_H
#define INFO_H

#include <vector>
#include <gtkmm.h>

/** This namespace holds some global informations about the program */
namespace INFO
{

/** The name of the program */
const Glib::ustring PROGRAM_TITLE =
    "Photocrypt";

/** Current version of the program */
const Glib::ustring PROGRAM_VERSION =
    "0.3";

/** Array of project team members */
const Glib::ustring _a[3] = {
    "Manish Munikar (070-BCT-520)",
    "Dipesh Pandey (070-BCT-514)",
    "Brihat Ratna Bajracharya (070-BCT-513)" };

/** The list of team members as a vector */
const std::vector<Glib::ustring> PROGRAM_AUTHORS(_a, _a + 3);

/** Website of the project */
const Glib::ustring PROGRAM_WEBSITE =
    "https://github.com/mdbct/photocrypt";

/** Label for the project website */
const Glib::ustring PROGRAM_WEBSITE_LABEL =
    "@ Github";

/** A brief description of the program */
const Glib::ustring PROGRAM_DESCRIPTION =
    "A text-to-image steganography tool"

/** A dummy copyright notice of the program */
const Glib::ustring PROGRAM_COPYRIGHT =
    "© 2015. MDBCT";

/** The licence text of the program */
const Glib::ustring PROGRAM_LICENSE = 
    "Copyright (c) 2015, MDBCT\n"
    "All rights reserved.\n"
    "\n"
    "Redistribution and use in source and binary forms, with or without\n"
    "modification, are permitted provided that the following conditions are met:\n"
    "\n"
    "  1. Redistributions of source code must retain the above copyright\n"
    "     notice, this list of conditions and the following disclaimer.\n"
    "  2. Redistributions in binary form must reproduce the above copyright\n"
    "     notice, this list of conditions and the following disclaimer in the\n"
    "     documentation and/or other materials provided with the distribution.\n"
    "  3. Neither the name of the MDBCT nor the\n"
    "     names of its contributors may be used to endorse or promote products\n"
    "     derived from this software without specific prior written permission.\n"
    "\n"
    "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n"
    "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
    "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
    "DISCLAIMED. IN NO EVENT SHALL MDBCT BE LIABLE FOR ANY\n"
    "DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n"
    "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n"
    "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND\n"
    "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
    "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
    "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

} // namespace INFO

#endif // INFO_H
