#include "Win.h"

using Glib::RefPtr;
using Gtk::Application;

int main(int argc, char** argv)
{
    RefPtr<Application> app = Application::create(argc, argv, "mdbct.photocrypt");

    Win w;

    return app -> run(w);
}
