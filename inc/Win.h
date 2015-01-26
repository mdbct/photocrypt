/** @file
 *  This file contains the decaration for the `Win` class which is
 *  the main window for our pgoram.
 */
#pragma once

#include <gtkmm.h>
#include "MatImage.h"
#include "TextFile.h"

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
        /** Displays a FileChooserDialog to open an image */
        virtual void onOpenImage();
        /** Displays a FileChooserDialog to open a text file */
        virtual void onOpenText();
        /** Closes window */
        virtual void close() { hide(); }
        /** Handler for image resize */
        virtual void onImageResize(Gtk::Allocation& a);
        /** On TextBuffer change */
        virtual void onTextBufferChange();
        /** Select steg-mode */
        virtual void onModeSteg();
        /** Select unsteg-mode */
        virtual void onModeUnsteg();
        /** Steg */
        virtual void steg();
        /** Unsteg */
        virtual void unsteg();
        /** Save text file */
        virtual void save();

        // Member widgets

        Glib::RefPtr<Gtk::ActionGroup>  mrActionGroup;
        Glib::RefPtr<Gtk::UIManager>    mrUIManager;

        Gtk::VBox mVBoxMain;

        Gtk::HBox mHBoxMain;
        Gtk::Statusbar mStatusbar;

        Gtk::VBox mVBoxImage;
        Gtk::VBox mVBoxText;
        Gtk::VSeparator mVSep;
        
        // Left
        Gtk::HBox         mHBoxImage;
        Gtk::AspectFrame  mFrameImage;
        Gtk::Image        mImage;

        Gtk::Button       mButtonOpenImage;
        Gtk::Label        mLabelImage;

        // Right
        Gtk::HBox           mHBoxText;
        Gtk::ScrolledWindow mScrolledWindowText;
        Gtk::TextView       mTextView;
        Glib::RefPtr<Gtk::TextBuffer> mrTextBuffer;
        Gtk::HBox           mHBoxKey;
        Gtk::HSeparator     mHSep;
        Gtk::HBox           mHBoxSave;

        Gtk::Button         mButtonOpenText;
        Gtk::Label          mLabelText;

        Gtk::Label mLabelKey;
        Gtk::Entry mEntryKey;

        Gtk::Button mButtonSteg;
        Gtk::Button mButtonSave;

        MatImage mMatImage;
        TextFile mTextFile;

        bool    mSized;
        sigc::connection mConnection;
};