/** @file
 *  This file contains the decaration for the `Win` class which is
 *  the main window for our pgoram.
 */
#pragma once

#include <gtkmm.h>

/**
 *  This class represents our main window
 */
class Win : public Gtk::Window
{
    public:

        /** Creates the window and all its childs */
        Win();
        /** Destructor, does nothing */
        virtual ~Win();

    protected:
        // Signal handlers

        /** Displays the AboutDialog */
        virtual void onActionAbout();
        /** Displays a simple MessageDialog */
        virtual void onAction(const std::string msg);
        /** Displays a FileChooserDialog to select an image */
        virtual void onButtonSelectImageClicked();
        /** Closes window */
        virtual void close() { hide(); }

        // Member widgets

        Gtk::VBox      mMainBox;            ///<  Main outermost box
        Gtk::Image     mImage;              ///<  The image view
        Gtk::Button    mButtonSelectImage;  ///< The button to select image
        Gtk::Statusbar mStatusbar;          ///< The statusbar

        Glib::RefPtr<Gtk::ActionGroup>  mrActionGroup; ///< The action group
        Glib::RefPtr<Gtk::UIManager>    mrUIManager;   ///< The UI Manager
};
