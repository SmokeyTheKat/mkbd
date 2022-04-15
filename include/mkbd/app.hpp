#ifndef __MKBD_APP_HPP__
#define __MKBD_APP_HPP__

#include <mkbd/gui/window.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/audioplayer.hpp>
#include <mkbd/midi/recorder.hpp>
#include <mkbd/instruments.hpp>

#include <string>
#include <vector>

class App {
	std::vector<std::string> mArgs;
	Window mWindow;

	Color mFgColor = Color(186, 238, 255);
	Color mBgColor = Color(176, 205, 219);
	Color mAccColor = Color(8, 119, 188);
	Color mBorderColor = Color(150, 150, 150);

	AudioPlayer mAudioPlayer;
	MidiRecorder mRecorder;

	int mMidiPort = 1;
	int mHeaderHeight = 60;
	int mPianoControlHeight = 60;

	Generator* mActiveGen = &pianoGen;

	GraphicPage mSettingsPage;

	int mMenuWidth = 300;

public:
	App(int argc, char** argv);
	int main(void);

private:
	int getWidth(void) { return mWindow.getWidth(); };
	int getHeight(void) { return mWindow.getHeight(); };

	std::string getChord(void);

	void attachRecorderToAudioPlayer(void);

	void generateHeader(const char* title);
	void generateFooter(void);

	void mainMenuPage(void);
	void generateMainSongView(void);
	void generateMainMenuHeader(void);
	void generateMainMenuButtons(void);

	void testPage(void);
	void freePlayPage(void);
	void generatePianoControls(void);

	void settingsPage(void);
	void settingsPageTabMidiDevice(void);
	void settingsPageTabMidiForwarding(void);
	void settingsPageTabColorScheme(void);
	void settingsPageInstruments(void);
	void settingsPageTabProformance(void);
	void settingsPageTabAbout(void);

	void newSettingsPage(void);

	void chooseKeyboardPage(void);
};

#endif