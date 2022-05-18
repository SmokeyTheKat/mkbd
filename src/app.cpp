#include <mkbd/app.hpp>

#include <mkbd/midi/keyboard.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/midi/recorder.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/instruments.hpp>
#include <mkbd/audioplayer.hpp>
#include <mkbd/generator.hpp>
#include <mkbd/pages.hpp>
#include <mkbd/gui/graphics.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/virtualkeyboard.hpp>
#include <mkbd/settings.hpp>

using namespace std::placeholders;

App::App(int argc, char** argv)
: mWindow(1280, 640) {
	for (int i = 0; i < argc; i++)
		mArgs.push_back(argv[i]);
}

int App::main(void) {
	mWindow.setBgColor(RGB_ARGS(gConfig.bgColor));

	pianoInstrument.load();
	piano2Instrument.load();
	piano2Instrument.setVolume(0.2);

//    mainMenuPage2(mWindow);

	mainMenuPage();

	return 0;
}

std::string App::getChord(void) {
	std::vector<int> notes;
	for (int i = 0; i < 256; i++) {
		if (mRecorder.getDevice()->getNoteState(i))
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

void App::generateFooter(void) {
	RectangleComponent* bottomBar = new RectangleComponent(
		Layout(
			0, 0,
			0, mHeaderHeight,
			Layout::FillX | Layout::AnchorBottomLeft
		),
		gConfig.accColor
	);
	mWindow.addComponent(bottomBar);
}

void App::generateHeader(const char* title) {
	int gap = mHeaderHeight / 4;

	RectangleComponent* topBar = new RectangleComponent(
		Layout(0, 0, 0, mHeaderHeight, Layout::FillX),
		gConfig.accColor
	);

	TextComponent* titleText = new TextComponent(
		Layout(0, 0, 0, mHeaderHeight, Layout::FillX),
		title,
		RESOURCE_DIR "/fonts/FreeSans.ttf",
		30,
		gConfig.fgColor
	);
	titleText->setParent(topBar);
	titleText->setAlign(FC_ALIGN_CENTER);

	ButtonComponent* backButton = new ButtonComponent(
		Layout(10, gap, 70, mHeaderHeight - gap * 2),
		"Back",
		[](){},
		gConfig.accColor, gConfig.fgColor
	);
	backButton->on("Click", asFunction<int, int>([this, backButton](int x, int y) {
		std::cout << backButton->getX() << " " << backButton->getY() << "\n";
		mWindow.popPage();
	}));
	backButton->setParent(topBar);

	mWindow.addComponent(topBar);
}

void App::mainMenuPage(void) {
	mWindow.newPage();

	generateMainMenuHeader();
	generateMainMenuButtons();
	generateMainSongView();
	generateFooter();

	RectangleComponent* background = new RectangleComponent(
		Layout(
			200, 200,
			200, 200
		), gConfig.accColor, gConfig.borderColor
	);

	Component* test = new RectangleComponent(
		Layout(100, 100, 600, 400),
		Color(255, 255, 0)
	);


	Component* wow = new RectangleComponent(Layout(0, 0, 150, 150, Layout::AnchorCenter), Color(255, 0, 0));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorCenter), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorTopCenter), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorBottomCenter), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorCenterLeft), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorCenterRight), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorTopLeft), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorTopRight), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorBottomLeft), Color(0, 0, 255)));
	wow->addChild(new RectangleComponent(Layout(0, 0, 25, 25, Layout::AnchorBottomRight), Color(0, 0, 255)));


	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorTopCenter), Color(255, 0, 0)));
	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorBottomCenter), Color(255, 0, 0)));
	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorCenterLeft), Color(255, 0, 0)));
	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorCenterRight), Color(255, 0, 0)));
	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorTopLeft), Color(255, 0, 0)));
	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorTopRight), Color(255, 0, 0)));
	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorBottomLeft), Color(255, 0, 0)));
	test->addChild(new RectangleComponent(Layout(0, 0, 50, 50, Layout::AnchorBottomRight), Color(255, 0, 0)));
	test->addChild(wow);

	mWindow.addComponent(test);

	mWindow.pageLoop();
}

void App::generateMainSongView(void) {
	int textHeight = 20;
	RectangleComponent* background = new RectangleComponent(
		Layout(
				mMenuWidth + 20, mHeaderHeight + textHeight + 20,
				mMenuWidth + 40, 2 * mHeaderHeight + textHeight + 40,
				Layout::FillX | Layout::FillY
		), gConfig.fgColor, gConfig.borderColor
	);

	TextComponent* text = new TextComponent(
		Layout(
			mMenuWidth + 20, mHeaderHeight + 20 - textHeight / 2,
			0, textHeight
		),
		"Your Songs", RESOURCE_DIR "/fonts/FreeSans.ttf", textHeight, Color(100, 100, 100)
	);

	mWindow.addComponent(text);
	mWindow.addComponent(background);
}

void App::generateMainMenuHeader(void) {
	int gap = mHeaderHeight / 8;

	RectangleComponent* topBar = new RectangleComponent(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), gConfig.accColor);
	ImageComponent* logo = new ImageComponent(Layout(10, gap, 100 - gap * 2, mHeaderHeight - gap * 2), RESOURCE_DIR "/images/logo.png");

	mWindow.addComponent(topBar);
	mWindow.addComponent(logo);
}

void App::testPage(void) {
	SET_FID;

	chooseKeyboardPage();

	mWindow.newPage();

	MidiDevice piano(mMidiPort);
	mRecorder = MidiRecorder(&piano, 120);

	mAudioPlayer.start();

	SheetMusicComponent* smg = new SheetMusicComponent(
		Layout(0, 0, 0, 300, Layout::FillX),
		&mRecorder
	);
	mWindow.addComponent(smg);

	KeyboardComponent* kg = new KeyboardComponent(
		Layout(0, 300, 0, 300, Layout::FillX | Layout::FillY),
		&mRecorder
	);
	mWindow.addComponent(kg);

	TextComponent* tg = new TextComponent(
		Layout(0, 40, 0, 100),
		"Cmaj9",
		RESOURCE_DIR "/fonts/FreeSans.ttf",
		50,
		Color(255, 255, 255)
	);
	mWindow.addComponent(tg);

	mRecorder.on("Update", asFunction(std::bind(&Window::update, &mWindow)));

	VirtualKeyboard vk(&mRecorder);
	mWindow.on("KeyDown", asFunction<int>(std::bind(&VirtualKeyboard::onKeyDown, &vk, _1)), FID);
	mWindow.on("KeyUp", asFunction<int>(std::bind(&VirtualKeyboard::onKeyUp, &vk, _1)), FID);

	vk.on("KeyDown", asFunction<byte>([&piano](byte note) {
		piano.sendEvent(MidiEvent({MidiEvent::NoteOn, note, 100}));
	}), FID);
	vk.on("KeyUp", asFunction<byte>([&piano](byte note) {
		piano.sendEvent(MidiEvent({MidiEvent::NoteOff, note, 0}));
	}), FID);

	mRecorder.on("NoteOn", asFunction<byte, byte>([this, &tg](byte note, byte velocity) {
		tg->setText(getChord());
	}));

	mRecorder.on("NoteOff", asFunction<byte>([this, &tg](byte note) {
		tg->setText(getChord());
	}));

	attachRecorderToAudioPlayer();

	mAudioPlayer.unpause();

	mRecorder.record(0);

	mWindow.pageLoop();

	mWindow.clearGroup(FID);
}

void App::generateMainMenuButtons(void) {
	ButtonListComponent* buttonList = new ButtonListComponent(
		Layout(10, mHeaderHeight + 20, mMenuWidth, 50),
		10,
		{
			{ "Free Play", std::bind(&App::freePlayPage, this) },
			{ "Test", std::bind(&App::testPage, this) },
			{ "Settings", std::bind(&App::settingsPage, this) },
			{ "Exit", [this](void){
				mWindow.popPage();
			} },
		},
		25, Colors::White, gConfig.accColor
	);

	mWindow.addComponent(buttonList);
}

void App::settingsPageTabAbout(void) {
	
}

void App::attachRecorderToAudioPlayer(void) {
	mRecorder.on("SustainChange", asFunction<byte>([this](byte sustain) {
		if (sustain == 0) {
			mAudioPlayer.sustainOff();
		} else {
			mAudioPlayer.sustainOn();
		}
	}));

	mRecorder.on("NoteOn", asFunction<byte, byte>([this](byte note, byte velocity) {
		mAudioPlayer.noteOn(*mActiveGen, Music::noteToFreq(note), rmap(velocity, 0, 127, 0, 50));
	}));

	mRecorder.on("NoteOff", asFunction<byte>([this](byte note) {
		mAudioPlayer.noteOff(Music::noteToFreq(note));
	}));
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

	ButtonListComponent* buttonList = new ButtonListComponent(
		Layout(
			0, mHeaderHeight + 20,
			buttonWidth, buttonHeight,
			Layout::AnchorTopCenter
		),
		buttonGap, buttons, 20, Colors::White, gConfig.accColor
	);

	mWindow.addComponent(buttonList);

	generateFooter();

	mWindow.pageLoop();
}
