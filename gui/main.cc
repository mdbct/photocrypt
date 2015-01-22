#include "Win.h"
#include <gtkmm/main.h>

int main(int argc, char** argv)
{
    Gtk::Main kit(argc, argv);

    Win w;

    Gtk::Main::run(w);

    return 0;
}
