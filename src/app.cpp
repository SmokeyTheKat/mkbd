#include <mkbd/app.hpp>

#include <mkbd/keyboard.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/recorder.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/instruments.hpp>
#include <mkbd/audioplayer.hpp>
#include <mkbd/generator.hpp>
#include <mkbd/gui/graphics/keyboard.hpp>
#include <mkbd/gui/graphics/list.hpp>
#include <mkbd/gui/graphics/rectangle.hpp>
#include <mkbd/gui/graphics/input.hpp>
#include <mkbd/gui/graphics/image.hpp>
#include <mkbd/gui/graphics/sheetmusic.hpp>
#include <mkbd/gui/graphics/button.hpp>
#include <mkbd/gui/graphics/buttonlist.hpp>
#include <mkbd/gui/graphics/waterfall.hpp>
#include <mkbd/gui/graphics/text.hpp>
#include <mkbd/gui/window.hpp>

static std::string getChord(MidiRecorder* rcdr) {
	std::vector<int> notes;
	for (int i = 0; i < 256; i++) {
		if (rcdr->getDevice()->getNoteState(i))
			notes.push_back(i);
	}

	std::string name;
	bool found = false;
	while (!found && notes.size() > 0) {
		name = Music::getChordName(notes);
		if (name.length() > 0)
			found = true;
		notes.pop_back();
	}
	return name;
}

App::App(int argc, char** argv)
: mWindow(1280, 640) {
	for (int i = 0; i < argc; i++)
		mArgs.push_back(argv[i]);
}

int App::main(void) {
	mWindow.setBgColor(RGB_ARGS(mBgColor));

	pianoInstrument.load();

	mainMenuPage();

	return 0;
}

void App::generateFooter(void) {
	RectangleGraphic* bottomBar = new RectangleGraphic(Layout(0, mHeaderHeight, 0, mHeaderHeight, Layout::FillX | Layout::AnchorBottomLeft), mAccColor);
	mWindow.addGraphic(bottomBar);
}

void App::generateHeader(const char* title) {
	int gap = mHeaderHeight / 4;
	RectangleGraphic* topBar = new RectangleGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), mAccColor);
	TextGraphic* titleText = new TextGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), title, RESOURCE_DIR "/fonts/FreeSans.ttf", 30, mFgColor);
	titleText->setAlign(FC_ALIGN_CENTER);

	ButtonGraphic* backButton = new ButtonGraphic(Layout(10, gap, 70, mHeaderHeight - gap * 2), "Back", [this](void) {
		mWindow.popPage();
	}, Color(4, 92, 149), mFgColor);
	backButton->setFontSize(17);

	mWindow.addGraphic(topBar);
	mWindow.addGraphic(backButton);
	mWindow.addGraphic(titleText);
}

void App::mainMenuPage(void) {
	mWindow.newPage();

	generateMainMenuHeader();
	generateMainMenuButtons();
	generateMainSongView();
	generateFooter();

	mWindow.pageLoop();
}

void App::generateMainSongView(void) {
	int textHeight = 20;
	RectangleGraphic* background = new RectangleGraphic(
		Layout(
				mMenuWidth + 20, mHeaderHeight + textHeight + 20,
				mMenuWidth + 40, 2 * mHeaderHeight + textHeight + 40,
				Layout::FillX | Layout::FillY
		), mFgColor, mBorderColor
	);

	TextGraphic* text = new TextGraphic(
		Layout(
			mMenuWidth + 20, mHeaderHeight + 20 - textHeight / 2,
			0, textHeight
		),
		"Your Songs", RESOURCE_DIR "/fonts/FreeSans.ttf", textHeight, Color(100, 100, 100)
	);

	mWindow.addGraphic(text);
	mWindow.addGraphic(background);
}

void App::generateMainMenuHeader(void) {
	int gap = mHeaderHeight / 8;

	RectangleGraphic* topBar = new RectangleGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), mAccColor);
	ImageGraphic* logo = new ImageGraphic(Layout(10, gap, 100 - gap * 2, mHeaderHeight - gap * 2), RESOURCE_DIR "/images/logo.png");

	mWindow.addGraphic(topBar);
	mWindow.addGraphic(logo);
}

void App::generateMainMenuButtons(void) {
	ButtonListGraphic* buttonList = new ButtonListGraphic(
		Layout(10, mHeaderHeight + 20, mMenuWidth, 50),
		10,
		{
			{ "Free Play", std::bind(&App::freePlayPage, this) },
			{ "Settings", std::bind(&App::settingsPage, this) },
			{ "Exit", [this](void){
				mWindow.popPage();
			} },
		},
		25, Colors::White, mAccColor
	);

	mWindow.addGraphic(buttonList);
}

void App::newSettingsPage(void) {
	for (Graphic* g : mSettingsPage) {
		mWindow.removeGraphic(g);
		delete g;
	}
	mSettingsPage.clear();
}

void App::settingsPageTabMidiDevice(void) {
	newSettingsPage();

	std::vector<GraphicCreater> creaters;

	std::vector<MidiDevice::Info> keyboards = MidiDevice::getDevices();
	for (auto& ki : keyboards) {
		int port = ki.port;
		creaters.push_back([this, port, ki](Layout layout) -> Graphic* {
			return new ButtonGraphic(layout, ki.name, [this, port](void) {
				mMidiPort = port;
				mWindow.popPage();
			}, mAccColor, Colors::White);
		});
	}

	int buttonWidth = 500;
	int buttonHeight = 35;
	int buttonGap = 5;

	std::vector<Graphic*> buttons = createListGraphic(
		Layout(buttonWidth / -2, mHeaderHeight + 20, buttonWidth, buttonHeight, Layout::AnchorTopCenter),
		creaters, 5
	);

	for (auto g : buttons) {
		mWindow.addGraphic(g);
		mSettingsPage.push_back(g);
	}
}

void App::settingsPageTabMidiForwarding(void) {
	newSettingsPage();

	RectangleGraphic* rect = new RectangleGraphic(Layout(mMenuWidth + 20, mHeaderHeight + 20, 200, 400), Color(255, 255, 0), Color(0, 0, 0));
	
	mSettingsPage.push_back(rect);
	mWindow.addGraphic(rect);
}

void App::settingsPageTabColorScheme(void) {
}

void App::settingsPageInstruments(void) {
}

void App::settingsPageTabProformance(void) {
	newSettingsPage();

	InputGraphic* input = new InputGraphic(Layout(mMenuWidth + 20, mHeaderHeight + 20, 200, 40), "128", [](void){}, InputType::Number);
	
	mSettingsPage.push_back(input);
	mWindow.addGraphic(input);
}

void App::settingsPageTabAbout(void) {
	
}

void App::settingsPage(void) {
	mWindow.newPage();

	mSettingsPage.clear();

	generateHeader("Settings");

	RectangleGraphic* rect = new RectangleGraphic(Layout(0, mHeaderHeight, mMenuWidth, mHeaderHeight - 1, Layout::FillY), mFgColor, Color(0, 0, 0));

	std::vector<Graphic*> buttons = createListGraphic(
		Layout(0, mHeaderHeight, mMenuWidth, 55),
		{
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Midi Device", std::bind(&App::settingsPageTabMidiDevice, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Midi Forwarding", std::bind(&App::settingsPageTabMidiForwarding, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Color Scheme", std::bind(&App::settingsPageTabColorScheme, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Instruments", std::bind(&App::settingsPageInstruments, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Proformance", std::bind(&App::settingsPageTabProformance, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "About", std::bind(&App::settingsPageTabAbout, this), mFgColor, Colors::Black);
			},
		}, -1
	);

	mWindow.addGraphic(rect);
	for (auto g : buttons)
		mWindow.addGraphic(g);

	mWindow.pageLoop();
}

void App::freePlayPage(void) {
	chooseKeyboardPage();

	mWindow.newPage();

	MidiDevice piano(mMidiPort);
	MidiRecorder recorder(&piano, 120);

	mAudioPlayer.start();

	int smHeight = 300;
	int kgHeight = 300;

	WaterfallGraphic* smg = new WaterfallGraphic(Layout(0, 0, 0, 300, Layout::FillX | Layout::FillY), &recorder);
	KeyboardGraphic* kg = new KeyboardGraphic(Layout(0, kgHeight, 0, kgHeight, Layout::FillX | Layout::AnchorBottomLeft), &recorder);
	TextGraphic* tg = new TextGraphic(Layout(0, 40, 0, 100), "Cmaj9", RESOURCE_DIR "/fonts/FreeSans.ttf", 50, Color(255, 255, 255));

	int buttonX = 10;
	int buttonY = 10;
	int buttonWidth = 100;
	int buttonHeight = 30;
	int buttonGap = 10;

	ButtonGraphic* butPiano = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Piano", [this](void) {
		mActiveGen = &pianoGen;
	}, mAccColor, mFgColor);
	butPiano->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butOrgan = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Organ", [this](void) {
		mActiveGen = &organGen;
	}, mAccColor, mFgColor);
	butOrgan->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butBrass = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Brass", [this](void) {
		mActiveGen = &brassGen;
	}, mAccColor, mFgColor);
	butBrass->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butReed = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Reed", [this](void) {
		mActiveGen = &reedGen;
	}, mAccColor, mFgColor);
	butReed->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butPhone = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Phone", [this](void) {
		mActiveGen = &phoneGen;
	}, mAccColor, mFgColor);
	butPhone->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butSettings = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Settings", [this, &recorder](void) {
		settingsPage();
	}, mAccColor, mFgColor);
	butSettings->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butQuit = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Quit", [this, &recorder](void) {
		recorder.stop();
		mAudioPlayer.stop();
		mWindow.popPage();
	}, mAccColor, mFgColor);
	butQuit->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	mWindow.onExit = [this, &recorder](Window* window) {
		recorder.stop();
		mAudioPlayer.stop();
		window->popPage();
	};

	recorder.onUpdate = [this](MidiRecorder* rcdr) {
		mWindow.update();
	};

	recorder.onSustainChange = [this](MidiRecorder* rcdr, MidiEvent msg) {
		if (msg[2] == 0) mAudioPlayer.sustainOff();
		else mAudioPlayer.sustainOn();
	};

	recorder.onSoftPedalDown = [this](MidiRecorder* rcdr, MidiEvent msg) {
		mAudioPlayer.stop();
		rcdr->stop();
		mWindow.popPage();
	};

	recorder.onPadDown = [this](MidiRecorder* rcdr, MidiEvent msg) {
		if (msg[1] != 16) return;
		mAudioPlayer.stop();
		rcdr->stop();
		mWindow.popPage();
	};

	recorder.onKeyDown = [this, &kg, &tg](MidiRecorder* rcdr, MidiEvent msg) {
		kg->keys[msg[1] - 12] = 1;
		tg->setText(getChord(rcdr));
		mWindow.update();
		mAudioPlayer.addSample(*mActiveGen, Music::noteToFreq(msg[1]), rmap(msg[2], 0, 127, 0, 50));
	};

	recorder.onKeyUp = [this, &kg, &tg](MidiRecorder* rcdr, MidiEvent msg) {
		kg->keys[msg[1] - 12] = 0;
		tg->setText(getChord(rcdr));
		mWindow.update();
		mAudioPlayer.removeSample(Music::noteToFreq(msg[1]));
	};

	mWindow.addGraphic(smg);
	mWindow.addGraphic(kg);
	mWindow.addGraphic(tg);
	mWindow.addGraphic(butPiano);
	mWindow.addGraphic(butOrgan);
	mWindow.addGraphic(butBrass);
	mWindow.addGraphic(butReed);
	mWindow.addGraphic(butPhone);
	mWindow.addGraphic(butSettings);
	mWindow.addGraphic(butQuit);

	mWindow.update();

	mAudioPlayer.unpause();

	recorder.record(0);

	mWindow.pageLoop();
}

void App::chooseKeyboardPage(void) {
	mWindow.newPage();

	generateHeader("Choose Keyboard");

	std::vector<ButtonDetails> buttons;

	std::vector<MidiDevice::Info> keyboards = MidiDevice::getDevices();
	for (auto& ki : keyboards) {
		int port = ki.port;
		ButtonDetails bd { ki.name.c_str(),
			[this, port](void) {
				mMidiPort = port;
				mWindow.popPage();
			}
		};
		buttons.push_back(bd);
	}

	int buttonWidth = 500;
	int buttonHeight = 35;
	int buttonGap = 5;

	ButtonListGraphic* buttonList = new ButtonListGraphic(
		Layout(buttonWidth / -2, mHeaderHeight + 20, buttonWidth, buttonHeight, Layout::AnchorTopCenter),
		buttonGap, buttons, 20, Colors::White, mAccColor
	);

	mWindow.addGraphic(buttonList);

	generateFooter();

	mWindow.pageLoop();
}
