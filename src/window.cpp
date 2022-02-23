#include <mkbd/window.hpp>

#include <iostream>

MainWindow::MainWindow(void)
	: mChordViewButton("Chord View"), mSheetMusicButton("Sheet Music") {
	set_border_width(10);

	mChordViewButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::openChordViewWindow));
	mSheetMusicButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::openSheetMusicWindow));

	add(mChordViewButton);
	add(mSheetMusicButton);

	this->show();
}

MainWindow::~MainWindow(void) {
}

void MainWindow::openChordViewWindow(void) {
}

void MainWindow::openSheetMusicWindow(void) {
}
