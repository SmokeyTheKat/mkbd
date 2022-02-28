#include <mkbd/window.hpp>

#include <iostream>

MainWindow::MainWindow(void)
	: mOptionsBox(Gtk::ORIENTATION_HORIZONTAL, 0), mChordViewButton("Chord View"), mSheetMusicButton("Sheet Music") {
	set_border_width(10);

	mChordViewButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::openChordViewWindow));
	mSheetMusicButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::openSheetMusicWindow));

	mOptionsBox.pack_start(mChordViewButton, false, false, 0);
	mOptionsBox.pack_start(mSheetMusicButton, true, true, 0);

	add(mOptionsBox);

	show_all_children();
}

MainWindow::~MainWindow(void) {
}

void MainWindow::openChordViewWindow(void) {
}

void MainWindow::openSheetMusicWindow(void) {
}
