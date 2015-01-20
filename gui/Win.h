/**
 * Declares the main window of the program.
 */
#pragma once

#include <gtkmm.h>

/**
 * Our main window.
 */
class Win : public Gtk::Window
{
    public:
        Win();
        virtual ~Win();

    protected:
        // Signal handlers
        virtual void onActionAbout();
        virtual void onAction(const std::string msg);
        virtual void onButtonSelectImageClicked();

        // Member widgets
        Gtk::VBox   mMainBox;
        Gtk::Image  mImage;
        Gtk::Button mButtonSelectImage;

        Glib::RefPtr<Gtk::ActionGroup>  mrActionGroup;
        Glib::RefPtr<Gtk::UIManager>    mrUIManager;
};
