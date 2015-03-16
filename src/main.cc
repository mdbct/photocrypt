/** @file
 *
 *  This is the starting point of the graphical program.
 */
#include <iostream>
#include <gtkmm.h>
#include "MatImage.h"
#include "Win.h"

using namespace std;

int main(int argc, char** argv)
{
    // Initialize GTK
    Gtk::Main kit(argc, argv);

    // Create the main window
    Win w;

    // Display the window
    Gtk::Main::run(w);

    return 0;
}
