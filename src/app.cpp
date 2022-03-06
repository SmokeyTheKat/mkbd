#include <mkbd/app.hpp>

#include <mkbd/keyboard.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/recorder.hpp>
#include <mkbd/player.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/audioplayer.hpp>
#include <mkbd/generator.hpp>
#include <mkbd/gui/graphics/keyboard.hpp>
#include <mkbd/gui/graphics/rectangle.hpp>
#include <mkbd/gui/graphics/image.hpp>
#include <mkbd/gui/graphics/sheetmusic.hpp>
#include <mkbd/gui/graphics/button.hpp>
#include <mkbd/gui/graphics/buttonlist.hpp>
#include <mkbd/gui/graphics/waterfall.hpp>
#include <mkbd/gui/graphics/text.hpp>
#include <mkbd/gui/window.hpp>

static const Color White(255, 255, 255);
static const Color Black(0, 0, 0);

static std::string getChord(KeyboardRecorder* rcdr) {
	std::vector<int> notes;
	for (int i = 0; i < 256; i++) {
		if (rcdr->getKeyboard()->getKeyState(i))
			notes.push_back(i);
	}
	return getChordName(notes);
}

static Generator pianoGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::piano(t, freq);
	},
	.release = LinearRelease<9500>,
	.fadeOut = LinearRelease<50>,
};

static Generator phoneGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::multi(t, freq, 1.5, 2.0, 0.4);
	},
};

static Generator brassGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * (Waves::pulse(t, freq, 25) + 0.25 * Waves::sawtooth(t, freq) + 0.0001 * Waves::noise());
	},
	.fadeOut = LinearRelease<200>,
};

static Generator reedGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * (Waves::triangle(t, freq * 2) + 0.5 * Waves::sine(t, freq) + 0.25 * Waves::sawtooth(t, freq) + 0.04 * Waves::noise());
	},
	.fadeOut = LinearRelease<200>,
};

static Generator organGen {
	.waveform = [](double t, double freq) -> double {
		return (0.5 * (Waves::sine(t, freq * 1.0) +
					  0.5 * Waves::sine(t, freq * 2.0) +
					  0.25 * Waves::sine(t, freq * 4.0) +
					  0.125 * Waves::sine(t, freq * 8.0)));
	},
	.fadeOut = LinearRelease<200>,
};

static Generator* activeGen = &organGen;

App::App(int argc, char** argv)
: mWindow(1280, 640) {
	for (int i = 0; i < argc; i++)
		mArgs.push_back(argv[i]);
}

int App::main(void) {
	mWindow.setBgColor(RGB_ARGS(mBgColor));
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
	TextGraphic* titleText = new TextGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), title, "fonts/FreeSans.ttf", 30, mFgColor);
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
	generateFooter();

	mWindow.pageLoop();
}

void App::generateMainMenuHeader(void) {
	int gap = mHeaderHeight / 8;

	RectangleGraphic* topBar = new RectangleGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), mAccColor);
	ImageGraphic* logo = new ImageGraphic(Layout(10, gap, 100 - gap * 2, mHeaderHeight - gap * 2), "./logo.png");

	mWindow.addGraphic(topBar);
	mWindow.addGraphic(logo);
}

void App::generateMainMenuButtons(void) {
	ButtonListGraphic* buttonList = new ButtonListGraphic(
		Layout(10, mHeaderHeight + 20, 300, 50),
		10,
		{
			{ "Free Play", std::bind(&App::freePlayPage, this) },
			{ "Settings", [](void){} },
			{ "Exit", [](void){} },
		},
		25, White, mAccColor
	);

	mWindow.addGraphic(buttonList);
}

void App::freePlayPage(void) {
	chooseKeyboardPage();

	mWindow.newPage();

	Keyboard piano(mMidiPort);
	KeyboardRecorder recorder(&piano, 120);

	AudioPlayer ap;
	ap.start();

	int smHeight = 300;
	int kgHeight = 300;

	WaterfallGraphic* smg = new WaterfallGraphic(Layout(0, 0, 0, 300, Layout::FillX | Layout::FillY), &recorder);
	KeyboardGraphic* kg = new KeyboardGraphic(Layout(0, kgHeight, 0, kgHeight, Layout::FillX | Layout::AnchorBottomLeft));
	TextGraphic* tg = new TextGraphic(Layout(0, 40, 0, 100), "Cmaj9", "fonts/FreeSans.ttf", 50, Color(255, 255, 255));

	int buttonX = 10;
	int buttonY = 10;
	int buttonWidth = 100;
	int buttonHeight = 30;
	int buttonGap = 10;

	ButtonGraphic* butPiano = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Piano", [this](void) {
		activeGen = &pianoGen;
	}, mAccColor, mFgColor);
	butPiano->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butOrgan = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Organ", [this](void) {
		activeGen = &organGen;
	}, mAccColor, mFgColor);
	butOrgan->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butBrass = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Brass", [this](void) {
		activeGen = &brassGen;
	}, mAccColor, mFgColor);
	butBrass->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butReed = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Reed", [this](void) {
		activeGen = &reedGen;
	}, mAccColor, mFgColor);
	butReed->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butPhone = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Phone", [this](void) {
		activeGen = &phoneGen;
	}, mAccColor, mFgColor);
	butPhone->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butSettings = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Settings", [this, &recorder, &ap](void) {
//        settingsPage();
	}, mAccColor, mFgColor);
	butSettings->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butQuit = new ButtonGraphic(Layout(buttonX, buttonY, buttonWidth, buttonHeight), "Quit", [this, &recorder, &ap](void) {
		recorder.stop();
		ap.stop();
		mWindow.popPage();
	}, mAccColor, mFgColor);
	butQuit->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	mWindow.onExit = [&ap, &recorder](Window* window) {
		recorder.stop();
		ap.stop();
		window->popPage();
	};

	recorder.onMessage = [this](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		mWindow.update();
	};

	recorder.onSustainChange = [this, &ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		if (msg[2] == 0) ap.sustainOff();
		else ap.sustainOn();
	};

	recorder.onSoftPedalDown = [this, &ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		ap.stop();
		rcdr->stop();
		mWindow.popPage();
	};

	recorder.onPadDown = [this, &ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		if (msg[1] != 16) return;
		ap.stop();
		rcdr->stop();
		mWindow.popPage();
	};

	recorder.onKeyDown = [this, &ap, &kg, &tg](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		kg->keys[msg[1] - 12] = 1;
		tg->setText(getChord(rcdr));
		mWindow.update();
//        ap.addSample(*activeGen, std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440, 50);
		ap.addSample(*activeGen, std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440, rmap(msg[2], 0, 127, 0, 50));
	};

	recorder.onKeyUp = [this, &ap, &kg, &tg](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		kg->keys[msg[1] - 12] = 0;
		tg->setText(getChord(rcdr));
		mWindow.update();
		ap.removeSample(std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440);
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

	ap.unpause();

	recorder.record(0);

	mWindow.pageLoop();
}

void App::chooseKeyboardPage(void) {
	mWindow.newPage();

	generateHeader("Choose Keyboard");

	std::vector<ButtonDetails> buttons;

	std::vector<KeyboardInfo> keyboards = Keyboard::getKeyboards();
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
		buttonGap, buttons, 20, White, mAccColor
	);

	mWindow.addGraphic(buttonList);

	generateFooter();

	mWindow.pageLoop();
}
