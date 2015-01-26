#include <iostream>
#include <gtkmm.h>
#include "MatImage.h"
#include "Win.h"

using namespace std;

int main(int argc, char** argv)
{
    Gtk::Main kit(argc, argv);

    Win w;

    Gtk::Main::run(w);

    return 0;
}
