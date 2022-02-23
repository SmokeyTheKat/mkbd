#ifndef __MKBD_WINDOW_HPP__
#define __MKBD_WINDOW_HPP__

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>

class MainWindow : public Gtk::Window {
public:
	MainWindow(void);
	virtual ~MainWindow(void);

protected:
	Gtk::Box mOptionsBox;
	Gtk::Button mChordViewButton;
	Gtk::Button mSheetMusicButton;

	void openSheetMusicWindow(void);
	void openChordViewWindow(void);
};

#endif