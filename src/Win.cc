/** @file
 *  This file defines the `Win` class
 */
#include "Win.h"
#include "info.h"
#include <iostream>
#include <string>
#include "MatImage.h"
#include "TextFile.h"

using namespace Gtk;
using Glib::RefPtr;
using Glib::ustring;
using sigc::mem_fun;
using sigc::bind;
using std::string;
using Gdk::Pixbuf;

// Default constructor for our main window
Win::Win() :
    mButtonSelectImage("Select Image...")
{
    // Set window parameters
    set_title(PROGRAM_TITLE);
    set_default_icon_from_file("icon.png");
    set_default_size(600, 400);

    add(mMainBox);

    // Create and fill the action group
    mrActionGroup = ActionGroup::create();

    mrActionGroup -> add( Action::create("ActionFile", "_File") );
    mrActionGroup -> add( Action::create("ActionEdit", "_Edit") );
    mrActionGroup -> add( Action::create("ActionHelp", "_Help") );

    mrActionGroup -> add( Action::create("ActionNew", Stock::NEW),
            bind(mem_fun(*this, &Win::onAction), "New") );
    mrActionGroup -> add( Action::create("ActionOpen", Stock::OPEN),
            mem_fun(*this, &Win::onButtonSelectImageClicked) );
    mrActionGroup -> add( Action::create("ActionSave", Stock::SAVE),
            bind(mem_fun(*this, &Win::onAction), "Save") );
    mrActionGroup -> add( Action::create("ActionSaveAs", Stock::SAVE_AS),
            AccelKey("<shift><control>s"),
            bind(mem_fun(*this, &Win::onAction), "Save As") );
    mrActionGroup -> add( Action::create("ActionClose", Stock::CLOSE),
            mem_fun(*this, &Win::close) );
    mrActionGroup -> add( Action::create("ActionQuit", Stock::QUIT),
            mem_fun(*this, &Win::close) );
    mrActionGroup -> add( Action::create("ActionCut", Stock::CUT),
            bind(mem_fun(*this, &Win::onAction), "Cut") );
    mrActionGroup -> add( Action::create("ActionCopy", Stock::COPY),
            bind(mem_fun(*this, &Win::onAction), "Copy") );
    mrActionGroup -> add( Action::create("ActionPaste", Stock::PASTE),
            bind(mem_fun(*this, &Win::onAction), "Paste") );
    mrActionGroup -> add( Action::create("ActionAbout", Stock::ABOUT),
            AccelKey("F1"),
            mem_fun(*this, &Win::onActionAbout) );

    // Create the UIManager and add the ActionGroup to it:
    mrUIManager = UIManager::create();
    mrUIManager -> insert_action_group(mrActionGroup);
    add_accel_group(mrUIManager -> get_accel_group());

    // Load the UI layout:
    string toolbar_layout = 
        "<ui>"
        "   <menubar name='MenuBar'>"
        "       <menu action='ActionFile'>"
        "           <menuitem action='ActionNew' />"
        "           <menuitem action='ActionOpen' />"
        "           <menuitem action='ActionSave' />"
        "           <menuitem action='ActionSaveAs' />"
        "           <separator />"
        "           <menuitem action='ActionClose' />"
        "           <menuitem action='ActionQuit' />"
        "       </menu>"
        "       <menu action='ActionEdit'>"
        "           <menuitem action='ActionCut' />"
        "           <menuitem action='ActionCopy' />"
        "           <menuitem action='ActionPaste' />"
        "       </menu>"
        "       <menu action='ActionHelp'>"
        "           <menuitem action='ActionAbout' />"
        "       </menu>"
        "   </menubar>"
        "   <toolbar name='Toolbar'>"
        "       <toolitem action='ActionNew' />"
        "       <toolitem action='ActionOpen' />"
        "       <toolitem action='ActionSave' />"
        "       <toolitem action='ActionQuit' />"
        "       <toolitem action='ActionAbout' />"
        "   </toolbar>"
        "</ui>";
    mrUIManager -> add_ui_from_string(toolbar_layout);

    // Add the MenuBar and Toolbar
    Widget* pMenuBar = mrUIManager -> get_widget("/MenuBar");
    Widget* pToolbar = mrUIManager -> get_widget("/Toolbar");

    mMainBox.pack_start(*pMenuBar, PACK_SHRINK);
    mMainBox.pack_start(*pToolbar, PACK_SHRINK);


    // Add a test image
    mMainBox.pack_start(mImage, PACK_EXPAND_WIDGET);

    // Add a separator before the "Open" button
    HSeparator mSeparator;
    mMainBox.pack_start(mSeparator, PACK_SHRINK);

    // Add a status bar:
    mStatusbar.push(PROGRAM_COPYRIGHT);
    mMainBox.pack_end(mStatusbar, PACK_SHRINK);

    // Add a button to select image to load
    mButtonSelectImage.set_border_width(10);
    mButtonSelectImage.signal_clicked().connect(mem_fun(*this, &Win::onButtonSelectImageClicked));
    mButtonSelectImage.set_can_default();
    mButtonSelectImage.grab_default();
    mMainBox.pack_end(mButtonSelectImage, PACK_SHRINK);

    show_all_children();
}


// Destructor
Win::~Win() {}


void Win::onActionAbout()
{
    AboutDialog d;
    d.set_transient_for(*this);

    d.set_program_name(PROGRAM_TITLE);
    d.set_version(PROGRAM_VERSION);
    d.set_authors(PROGRAM_AUTHORS);
    d.set_documenters(PROGRAM_AUTHORS);
    d.set_comments(PROGRAM_DESCRIPTION);
    d.set_website(PROGRAM_WEBSITE);
    d.set_website_label(PROGRAM_WEBSITE_LABEL);
    d.set_license(PROGRAM_LICENSE);
    d.set_copyright(PROGRAM_COPYRIGHT);

    d.run();
}

void Win::onAction(const string msg)
{
    MessageDialog d(msg);
    d.set_transient_for(*this);
    d.set_title("Message");
    d.run();
}

void Win::onButtonSelectImageClicked()
{
    // Create a FileChooseDialog with CANCEL & OK buttons
    FileChooserDialog d(*this, "Select an image");
    d.add_button(Stock::CANCEL, RESPONSE_CANCEL);
    d.add_button(Stock::OPEN, RESPONSE_OK);

    // Add filter to select only images
    FileFilter filterImage;
    filterImage.set_name("Image files");
    filterImage.add_pattern("*.jpg");
    filterImage.add_pattern("*.png");
    filterImage.add_pattern("*.bmp");
    filterImage.add_pattern("*.gif");

    d.add_filter(filterImage);

    if (d.run() == RESPONSE_OK)
    {
        string filename = d.get_filename();       

        MatImage I = filename;

        int w, h;
        this -> get_size(w, h);

        mImage.set(I.fit(w, h));
    }
}
