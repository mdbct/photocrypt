/** @file
 *  This file defines the `Win` class
 */
#include "Win.h"
#include "INFO.h"
#include <iostream>
#include <string>
#include "MatImage.h"
#include "TextFile.h"
#include "util.h"
#include "Error.h"

using namespace Gtk;
using Glib::RefPtr;
using Glib::ustring;
using sigc::mem_fun;
using sigc::bind;
using std::string;
using util::to_str;
using Gdk::Pixbuf;

// Default constructor for our main window
Win::Win() :
    mLabelKey("Enter password"),
    mLabelImage("Select an image"),
    mLabelText("Write message or open a file"),
    mSized(false),
    mButtonSave("Save Text"),
    mButtonOpenImage("Open an Image"),
    mButtonClearImage("Clear the Image"),
    mButtonOpenText("Open a Text File")
{
    // Set window parameters
    set_title(PROGRAM_TITLE);

    add(mVBoxMain);

    // Create and fill the action group
    mrActionGroup = ActionGroup::create();

    // Menus
    mrActionGroup -> add( Action::create("ActionFile", "_File") );
    mrActionGroup -> add( Action::create("ActionEdit", "_Edit") );
    mrActionGroup -> add( Action::create("ActionMode", "_Mode") );
    mrActionGroup -> add( Action::create("ActionHelp", "_Help") );

    // FileMenu
    mrActionGroup -> add( Action::create("ActionOpenImage", "Open _Image"),
            mem_fun(*this, &Win::onOpenImage) );
    mrActionGroup -> add( Action::create("ActionOpenText", "Open _Text File"),
            mem_fun(*this, &Win::onOpenText) );
    mrActionGroup -> add( Action::create("ActionQuit", Stock::QUIT),
            mem_fun(*this, &Win::close) );

    // EditMenu
    mrActionGroup -> add( Action::create("ActionCut", Stock::CUT),
            mem_fun(*this, &Win::onActionCut) );
    mrActionGroup -> add( Action::create("ActionCopy", Stock::COPY),
            mem_fun(*this, &Win::onActionCopy) );
    mrActionGroup -> add( Action::create("ActionPaste", Stock::PASTE),
            mem_fun(*this, &Win::onActionPaste) );

    // ModeMenu
    RadioAction::Group groupMode;
    mrActionGroup -> add( RadioAction::create(groupMode, "ActionModeSteg", "Steg"),
            AccelKey("<control>1"),
            mem_fun(*this, &Win::onModeSteg) );
    mrActionGroup -> add( RadioAction::create(groupMode, "ActionModeUnsteg", "Unsteg"),
            AccelKey("<control>2"),
            mem_fun(*this, &Win::onModeUnsteg) );

    // HelpMenu
    mrActionGroup -> add( Action::create("ActionAbout", Stock::ABOUT),
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
        "           <menuitem action='ActionOpenImage' />"
        "           <menuitem action='ActionOpenText' />"
        "           <separator />"
        "           <menuitem action='ActionQuit' />"
        "       </menu>"
        "       <menu action='ActionEdit'>"
        "           <menuitem action='ActionCut' />"
        "           <menuitem action='ActionCopy' />"
        "           <menuitem action='ActionPaste' />"
        "       </menu>"
        "       <menu action='ActionMode'>"
        "           <menuitem action='ActionModeSteg' />"
        "           <menuitem action='ActionModeUnsteg' />"
        "       </menu>"
        "       <menu action='ActionHelp'>"
        "           <menuitem action='ActionAbout' />"
        "       </menu>"
        "   </menubar>"
        //"   <toolbar name='Toolbar'>"
        //"       <toolitem action='ActionOpenImage' />"
        //"       <toolitem action='ActionOpenText' />"
        //"       <toolitem action='ActionQuit' />"
        //"       <toolitem action='ActionAbout' />"
        //"   </toolbar>"
        "</ui>";
    mrUIManager -> add_ui_from_string(toolbar_layout);

    // Add the MenuBar and Toolbar:
    Widget* pMenuBar = mrUIManager -> get_widget("/MenuBar");
    //Widget* pToolbar = mrUIManager -> get_widget("/Toolbar");

    // Manage Clipboard:
    mrClipboard = Clipboard::get();

    // Fill mVBoxMain:
    mVBoxMain.pack_start(*pMenuBar, PACK_SHRINK);
    //mVBoxMain.pack_start(*pToolbar, PACK_SHRINK);
    mVBoxMain.pack_start(mHBoxMain);
    mVBoxMain.pack_start(mStatusbar, PACK_SHRINK);
    mStatusbar.push(PROGRAM_TITLE + " " + PROGRAM_VERSION + " " + PROGRAM_COPYRIGHT);

    // Fill mHBoxMain:
    mHBoxMain.set_spacing(0);
    mHBoxMain.pack_start(mVBoxImage);
    mHBoxMain.pack_start(mVSep, PACK_SHRINK);
    mHBoxMain.pack_start(mVBoxText);

    // Fill mVBoxImage
    mVBoxImage.set_border_width(10);
    mVBoxImage.set_spacing(10);
    mVBoxImage.pack_start(mHBoxImage, PACK_SHRINK);
    mVBoxImage.pack_start(mFrameImage, PACK_EXPAND_WIDGET);

    // Fill mHBoxImage
    mHBoxImage.pack_start(mButtonOpenImage, PACK_SHRINK);
    mHBoxImage.pack_start(mButtonClearImage, PACK_SHRINK);
    mHBoxImage.pack_end(mLabelImage, PACK_SHRINK);

    // Fill mFrameImage
    mFrameImage.set_size_request(400, 400);
    mFrameImage.set(ALIGN_CENTER, ALIGN_CENTER, 1);
    mFrameImage.add(mImage);

    // Fill mVBoxText
    mVBoxText.set_border_width(10);
    mVBoxText.set_spacing(10);
    mVBoxText.pack_start(mHBoxText, PACK_SHRINK);
    mVBoxText.pack_start(mScrolledWindowText);
    mVBoxText.pack_start(mHBoxKey, PACK_SHRINK);
    mVBoxText.pack_start(mHSep, PACK_SHRINK);
    mVBoxText.pack_start(mHBoxSave, PACK_SHRINK);

    // Fill mHBoxText
    mHBoxText.pack_start(mButtonOpenText, PACK_SHRINK);
    mHBoxText.pack_end(mLabelText, PACK_SHRINK);

    // Fill mScrolledWindow
    mrTextBuffer = mTextView.get_buffer();
    mrTextBuffer -> signal_changed().connect(mem_fun(*this, &Win::onTextBufferChange));
    mScrolledWindowText.add(mTextView);

    // Fill mHBoxKey
    mHBoxKey.set_spacing(10);
    mHBoxKey.pack_start(mLabelKey, PACK_SHRINK);
    mHBoxKey.pack_start(mEntryKey);

    // Fill mHBoxSave
    mHBoxSave.set_spacing(10);
    mHBoxSave.set_size_request(400, 40);
    mHBoxSave.pack_start(mButtonSteg);
    mHBoxSave.pack_start(mButtonSave);

    // Widgets in the left side
    mButtonOpenImage.signal_clicked().connect(mem_fun(*this, &Win::onOpenImage));
    mButtonClearImage.signal_clicked().connect(mem_fun(*this, &Win::onClearImage));

    mImage.signal_size_allocate().connect(mem_fun(*this, &Win::onImageResize));

    // Widgets in the right side
    mButtonOpenText.signal_clicked().connect(mem_fun(*this, &Win::onOpenText));

    mScrolledWindowText.set_policy(POLICY_AUTOMATIC, POLICY_AUTOMATIC);

    mEntryKey.set_visibility(false);

    mButtonSteg.set_can_default();
    mButtonSteg.grab_default();

    mButtonSave.signal_clicked().connect(mem_fun(*this, &Win::save));

    onModeSteg();
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

void Win::onOpenImage()
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
        mMatImage = filename;
        mImage.set(mMatImage.fit(mImage.get_width(), mImage.get_height()));
        string label = "Capacity: " + to_str(mMatImage.max()) + " Bytes";
        mLabelImage.set_label(label);
    }
}

void Win::onClearImage()
{
    mMatImage = MatImage();
    mImage.clear();
    mLabelImage.set_label("Select an image");
}

void Win::onOpenText()
{
    FileChooserDialog d(*this, "Select a text file");
    d.add_button(Stock::CANCEL, RESPONSE_CANCEL);
    d.add_button(Stock::OPEN, RESPONSE_OK);

    FileFilter filterText;
    filterText.set_name("ASCII Text Files");
    filterText.add_mime_type("text/plain");
    d.add_filter(filterText);

    if (d.run() == RESPONSE_OK)
    {
        string filename = d.get_filename();
        mTextFile = TextFile::open(filename);
        mrTextBuffer -> set_text(mTextFile.str());
    }
}

void Win::onImageResize(Allocation& a)
{
    if (mMatImage.empty())
        return;

    if (not mSized) {
        mImage.set(mMatImage.fit(a.get_width(), a.get_height()));
        mSized = true;
    } else {
        mSized = false;
    }
}

void Win::onModeSteg()
{
    mButtonSteg.set_label("Steg");
    mConnection.disconnect();
    mConnection = mButtonSteg.signal_clicked().connect(mem_fun(*this, &Win::steg));
    mLabelKey.set_label("Set a password");

    mButtonSave.set_sensitive(false);
    mButtonOpenText.set_sensitive(true);
    mTextView.set_sensitive(true);
    mEntryKey.set_text("");
}

void Win::onModeUnsteg()
{
    mButtonSteg.set_label("Unsteg");
    mConnection.disconnect();
    mConnection = mButtonSteg.signal_clicked().connect(mem_fun(*this, &Win::unsteg));
    mLabelKey.set_label("Enter the password");

    mrTextBuffer -> set_text("");
    mTextView.set_sensitive(false);
    mButtonSave.set_sensitive(false);
    mButtonOpenText.set_sensitive(false);
    mEntryKey.set_text("");
}

void Win::steg()
{
    try
    {
        mMatImage.steg((mrTextBuffer->get_text()).raw(), mEntryKey.get_text().raw());
    }
    catch (ImageEmptyError)
    {
        display_error("Select an image first.");
        return;
    }
    catch (TextEmptyError)
    {
        display_error("There is no text to hide.");
        return;
    }
    catch (KeyEmptyError)
    {
        display_error("You must set a password.");
        return;
    }
    catch (InsufficientImageError)
    {
        display_error("The image is not large enough.");
        return;
    }

    FileChooserDialog d(*this, "Save stego image as...", FILE_CHOOSER_ACTION_SAVE);
    d.add_button(Stock::CANCEL, RESPONSE_CANCEL);
    d.add_button(Stock::SAVE, RESPONSE_OK);

    FileFilter filterImage;
    filterImage.set_name("BMP or PNG Images");
    filterImage.add_pattern("*.bmp");
    filterImage.add_pattern("*.png");
    d.add_filter(filterImage);

    if (d.run() == RESPONSE_OK) {
        mMatImage.save(d.get_filename().raw());
        mStatusbar.pop();
        mStatusbar.push("Stego-image written successfully.");
        mEntryKey.set_text("");
    }
}

void Win::unsteg()
{
    string text;
    try 
    {
        text = mMatImage.unsteg(mEntryKey.get_text().raw());
    }
    catch (ImageEmptyError)
    {
        display_error("Select an image first.");
        return;
    }
    catch (ImageNotStegoError)
    {
        display_error("The selected image is not a stego-image.");
        return;
    }
    catch (KeyEmptyError)
    {
        display_error("You must enter a password.");
        return;
    }
    catch (KeyMismatchError)
    {
        display_error("Incorrect password.");
        return;
    }

    mrTextBuffer -> set_text(text);
    mTextView.set_sensitive();
    mButtonSave.set_sensitive();
}

void Win::onTextBufferChange()
{
    int size = (mrTextBuffer -> get_text()).size();
    string label = "Size: " + to_str(size) + " Bytes";
    mLabelText.set_label(label);
}

void Win::save()
{
    FileChooserDialog d(*this, "Save text as...", FILE_CHOOSER_ACTION_SAVE);
    d.add_button(Stock::CANCEL, RESPONSE_CANCEL);
    d.add_button(Stock::SAVE, RESPONSE_OK);

    FileFilter filter;
    filter.set_name("ASCII Text Files");
    filter.add_mime_type("text/plain");
    d.add_filter(filter);

    if (d.run() == RESPONSE_OK) {
        string filename = d.get_filename().raw();
        TextFile t = (mrTextBuffer -> get_text()).raw();
        t.save(filename);

        mStatusbar.pop();
        mStatusbar.push("Text file saved successfully.");
        mEntryKey.set_text("");
    }
}

void Win::onActionCut()
{
    onActionCopy();
    mrTextBuffer -> erase_selection(false);
}

void Win::onActionCopy()
{
    TextBuffer::iterator start, end;
    if (mrTextBuffer->get_selection_bounds(start, end)) {
        ustring selectedText = mrTextBuffer->get_slice(start, end);
        mrClipboard -> set_text(selectedText);
    }
}

void Win::onActionPaste()
{
    mrClipboard -> request_text(mem_fun(*this, &Win::onClipboardTextRecieved));
}

void Win::onClipboardTextRecieved(const ustring& text)
{
    mrTextBuffer -> insert_at_cursor(text);
}

void Win::display_error(const string& msg)
{
    MessageDialog(*this, msg, MESSAGE_ERROR).run();
}
